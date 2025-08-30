#include "Time.hpp"

#include "SDL3/SDL_timer.h"
#include "spdlog/spdlog.h"

namespace engine::core
{

Time::Time()
{
    last_tick_ns_ = SDL_GetTicksNS();
    spdlog::trace("Time 初始化 last_tick_ns_: {}", last_tick_ns_);
}

void Time::update()
{
    Uint64 current_tick = SDL_GetTicksNS();
    Uint64 current_delta_tick = current_tick - last_tick_ns_;
    if (target_frame_ns_ > 0 && current_delta_tick < target_frame_ns_)
    {
        SDL_DelayNS(target_frame_ns_ - current_delta_tick);
        current_tick = SDL_GetTicksNS();
        delta_time_ns_ = current_tick - last_tick_ns_;
    }
    else
    {
        delta_time_ns_ = current_delta_tick;
    }

    last_tick_ns_ = current_tick;
}

float Time::getDeltaTime() const
{
    return (static_cast<float>(delta_time_ns_) / 1'000'000'000.0f) * time_scale_;
}
void Time::setTimeScale(float scale)
{
    time_scale_ = scale;
}
float Time::getTimeScale() const
{
    return time_scale_;
}
void Time::setTargetFps(int target_fps)
{
    if (target_fps < 0)
    {
        spdlog::warn("目标 FPS 值不能被设置为负");
        return;
    }

    target_fps_ = target_fps;

    if (target_fps == 0)
    {
        target_frame_ns_ = 0;
        return;
    }

    target_frame_ns_ = 1000'000'000 / target_fps_;
}

int Time::getTargetFps() const
{
    return target_fps_;
}

} // namespace engine::core
