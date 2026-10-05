#include "AudioEngine.hpp"
#include <atomic>
#include <cstddef>
#include <cstring>

constexpr double M_PI_M2(M_PI + M_PI);

constexpr int DEFAULT_RATE = 44100;
constexpr int DEFAULT_CHANNELS = 2;

const struct pw_stream_events AudioEngine::stream_events = {
    .version = PW_VERSION_STREAM_EVENTS,
    .process = on_process,
};

AudioEngine::AudioEngine(AudioState &state)
    : loop_(pw_main_loop_new(nullptr)),
      stream_(
          pw_stream_new_simple(pw_main_loop_get_loop(loop_), "audio-src",
                               pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio", PW_KEY_MEDIA_CATEGORY,
                                                 "Playback", PW_KEY_MEDIA_ROLE, "Music", nullptr),
                               &stream_events, this)),
      state_(state) {

    auto info = SPA_AUDIO_INFO_RAW_INIT(.format = SPA_AUDIO_FORMAT_S16, .rate = DEFAULT_RATE,
                                        .channels = DEFAULT_CHANNELS);

    b = SPA_POD_BUILDER_INIT(buffer.data(), sizeof(buffer));

    params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &info);

    pw_stream_connect(stream_, PW_DIRECTION_OUTPUT, PW_ID_ANY,
                      static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT |
                                                   PW_STREAM_FLAG_MAP_BUFFERS |
                                                   PW_STREAM_FLAG_RT_PROCESS),
                      reinterpret_cast<const struct spa_pod **>(params.data()), 1);
}

void AudioEngine::run() { pw_main_loop_run(loop_); }

AudioEngine::~AudioEngine() {
    if (stream_)
        pw_stream_destroy(stream_);

    if (loop_)
        pw_main_loop_destroy(loop_);
}

AudioEngine *AudioEngine::s_instance = nullptr;

/* [on_process] */
void AudioEngine::on_process(void *userdata) {
    s_instance = static_cast<AudioEngine *>(userdata);
    if (!s_instance)
        return;

    struct pw_buffer *b{};
    struct spa_buffer *buf{};
    int c{};
    uint32_t n_frames{};
    int32_t stride{};
    int16_t *dst{}, val{};

    b = pw_stream_dequeue_buffer(s_instance->stream_);
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

    bool muted = s_instance->state_.is_muted.load(std::memory_order_relaxed);
    float raw_vol = s_instance->state_.vol.load(std::memory_order_relaxed);
    float freq = s_instance->state_.freq.load(std::memory_order_relaxed);

    float vol = muted ? 0.0f : raw_vol;
    const float scale = 32767.0;

    for (uint32_t i = 0; i < n_frames; i++) {
        s_instance->accumulator_ += M_PI_M2 * freq / DEFAULT_RATE;
        if (s_instance->accumulator_ >= M_PI_M2)
            s_instance->accumulator_ -= M_PI_M2;

        /* sin() gives a value between -1.0 and 1.0, we first apply
         * the volume and then scale with 32767.0 to get a 16 bits value
         * between [-32767 32767].
         * Another common method to convert a double to
         * 16 bits is to multiple by 32768.0 and then clamp to
         * [-32768 32767] to get the full 16 bits range. */
        val = static_cast<int16_t>(sin(s_instance->accumulator_) * vol * scale);

        for (c = 0; c < DEFAULT_CHANNELS; c++)
            *dst++ = val;
    }

    buf->datas[0].chunk->offset = 0;
    buf->datas[0].chunk->stride = stride;
    buf->datas[0].chunk->size = n_frames * stride;

    pw_stream_queue_buffer(s_instance->stream_, b);
}
/* [on_process] */
