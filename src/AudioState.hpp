#pragma once
#include <atomic>

struct AudioState {
    std::atomic<float> freq{523.3f};
    std::atomic<bool> is_muted{true};
    std::atomic<float> vol{0.8f};
    std::atomic<uint32_t> samples_remaining{0};
};
