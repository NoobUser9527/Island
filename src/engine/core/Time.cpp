#include "Time.hpp"

#include "SDL3/SDL_timer.h"
#include "spdlog/spdlog.h"

namespace engine::core {

Time::Time() {
    last_time_ = SDL_GetTicksNS();
    frame_start_time_ = last_time_;
    spdlog::trace("Time 初始化。last_time: {}", last_time_);
}

void Time::update() {
    frame_start_time_ = SDL_GetTicksNS();
    auto current_delta_time = static_cast<double>(frame_start_time_ - last_time_) / 1000000000.0;
    if (target_frame_time_ > 0.0) {
        limitFrameRate(current_delta_time);
    } else {
        delta_time_ = current_delta_time;
    }

    last_time_ = SDL_GetTicksNS();
}

void Time::limitFrameRate(float current_delta_time) {
    if (current_delta_time < target_frame_time_) {
        double time_to_wait = target_frame_time_ - current_delta_time;
        Uint64 ns_to_wait = static_cast<double>(time_to_wait * 1 * 1000 * 1000 * 1000.0);
        SDL_DelayNS(ns_to_wait);
        delta_time_ = static_cast<double>(SDL_GetTicksNS() - last_time_) / 1 * 1000 * 1000 * 1000.0;
    }
}

float Time::getDeltaTime() const { return static_cast<float>(delta_time_ * time_scale_); }

float Time::getUnscaledDeltaTime() const { return delta_time_; }
void Time::setTimeScale(float scale) { time_scale_ = static_cast<double>(scale); }

float Time::getTimeScale() const { return static_cast<float>(time_scale_); }
void Time::setTargetFps(int target_fps) { target_fps_ = target_fps; }

} // namespace engine::core