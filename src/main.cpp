#include <array>
#include <cmath>

#include <spa/param/audio/format-utils.h>

#include <pipewire/pipewire.h>

constexpr double M_PI_M2(M_PI + M_PI);

constexpr int DEFAULT_RATE = 44100;
constexpr int DEFAULT_CHANNELS = 2;
constexpr double DEFAULT_VOLUME = 0.7;

struct data {
    struct pw_main_loop *loop;
    struct pw_stream *stream;
    double accumulator;
};

/* [on_process] */
static void on_process(void *userdata) {
    auto *data = static_cast<struct data *>(userdata);
    struct pw_buffer *b{};
    struct spa_buffer *buf{};
    int i{}, c{};
    uint32_t n_frames{};
    int32_t stride{};
    int16_t *dst{}, val{};

    b = pw_stream_dequeue_buffer(data->stream);
    if (b == nullptr) {
        pw_log_warn("out of buffers: %m");
        return;
    }

    buf = b->buffer;
    dst = static_cast<int16_t *>(buf->datas[0].data);
    if (dst == nullptr)
        return;

    stride = sizeof(int16_t) * DEFAULT_CHANNELS;
    n_frames = buf->datas[0].maxsize / stride;
    if (b->requested)
        n_frames = SPA_MIN(b->requested, n_frames);

    const double hertz = 440.0;

    for (i = 0; i < n_frames; i++) {
        data->accumulator += M_PI_M2 * hertz / DEFAULT_RATE;
        if (data->accumulator >= M_PI_M2)
            data->accumulator -= M_PI_M2;

        /* sin() gives a value between -1.0 and 1.0, we first apply
         * the volume and then scale with 32767.0 to get a 16 bits value
         * between [-32767 32767].
         * Another common method to convert a double to
         * 16 bits is to multiple by 32768.0 and then clamp to
         * [-32768 32767] to get the full 16 bits range. */
        val = static_cast<int16_t>(sin(data->accumulator) * DEFAULT_VOLUME * 32767.0);
        for (c = 0; c < DEFAULT_CHANNELS; c++)
            *dst++ = val;
    }

    buf->datas[0].chunk->offset = 0;
    buf->datas[0].chunk->stride = stride;
    buf->datas[0].chunk->size = n_frames * stride;

    pw_stream_queue_buffer(data->stream, b);
}
/* [on_process] */

static const struct pw_stream_events stream_events = {
    .version = PW_VERSION_STREAM_EVENTS,
    .process = on_process,
};

class AudioEngine {
  public:
    AudioEngine();
    ~AudioEngine();

    AudioEngine(const AudioEngine &) = delete;
    AudioEngine &operator=(const AudioEngine &) = delete;

    AudioEngine(AudioEngine &&) = delete;
    AudioEngine &operator=(AudioEngine &&) = delete;
};

int main(int argc, char *argv[]) {
    struct data data = {
        .loop = nullptr,
    };

    const struct spa_pod *params[1];
    std::array<int16_t, 1024> buffer{};
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer.data(), sizeof(buffer));

    pw_init(&argc, &argv);

    data.loop = pw_main_loop_new(nullptr);

    data.stream =
        pw_stream_new_simple(pw_main_loop_get_loop(data.loop), "audio-src",
                             pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio", PW_KEY_MEDIA_CATEGORY,
                                               "Playback", PW_KEY_MEDIA_ROLE, "Music", NULL),
                             &stream_events, &data);

    auto info = SPA_AUDIO_INFO_RAW_INIT(.format = SPA_AUDIO_FORMAT_S16, .rate = DEFAULT_RATE,
                                        .channels = DEFAULT_CHANNELS);

    params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &info);

    pw_stream_connect(data.stream, PW_DIRECTION_OUTPUT, PW_ID_ANY,
                      static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT |
                                                   PW_STREAM_FLAG_MAP_BUFFERS |
                                                   PW_STREAM_FLAG_RT_PROCESS),
                      static_cast<const struct spa_pod **>(params), 1);

    pw_main_loop_run(data.loop);

    pw_stream_destroy(data.stream);
    pw_main_loop_destroy(data.loop);

    return 0;
}
