#pragma once

#include <SDL3/SDL_stdinc.h>

namespace engine::core
{
class Time final
{
private:
    Uint64 last_tick_ns_ = 0;
    Uint64 delta_time_ns_ = 0;
    float time_scale_ = 1.0f;
    int target_fps_ = 0;
    Uint64 target_frame_ns_ = 0;

public:
    Time();

    Time(const Time &) = delete;
    Time &operator=(const Time &) = delete;
    Time(Time &&) = delete;
    Time &operator=(Time &&) = delete;

    void update();

    float getDeltaTime() const;

    void setTimeScale(float scale);
    float getTimeScale() const;

    void setTargetFps(int target_fps);
    int getTargetFps() const;
};
} // namespace engine::core
