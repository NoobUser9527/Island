#pragma once

#include <SDL3/SDL_stdinc.h>

namespace engine::core {
class Time final{
  private:
    Uint64 last_time_ = 0;
    Uint64 frame_start_time_ = 0;
    double delta_time_ = 0.0;
    double time_scale_ = 1.0;

    //
    int target_fps_ = 0;
    double target_frame_time_ = 0.0;

  public:
    Time();

    Time(const Time &) = delete;
    Time &operator=(const Time &) = delete;
    Time(Time &&) = delete;
    Time &operator=(Time &&) = delete;
    

    void update();

    float getDeltaTime() const;
    float getUnscaledDeltaTime() const;

    void setTimeScale(float scale);
    float getTimeScale() const;

    void setTargetFps(int target_fps);

  private:
    void limitFrameRate(float current_delta_time);
};
} // namespace engine::core