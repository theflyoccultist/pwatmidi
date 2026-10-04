#pragma once

#include <atomic>

struct AudioState {
    std::atomic<double> freq{260.0};
    std::atomic<bool> is_muted{false};
    std::atomic<float> vol{0.9f};
};
