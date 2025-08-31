#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource
{
class ResourceManager;
}

namespace engine::render
{
class Renderer;
class Camera;
}

namespace engine::core
{

class Time;

class GameApp final
{
private:
    //
    SDL_Window *window_ = nullptr;
    SDL_Renderer *sdl_renderer_ = nullptr;
    bool is_running_ = false;

    //
    std::unique_ptr<engine::core::Time> time_;
    std::unique_ptr<engine::resource::ResourceManager> resource_manager_;
    std::unique_ptr<engine::render::Renderer> renderer_;
    std::unique_ptr<engine::render::Camera> camera_;

public:
    GameApp();
    ~GameApp();

    void run();

    GameApp(const GameApp &) = delete;
    GameApp &operator=(const GameApp &) = delete;
    GameApp(GameApp &&) = delete;
    GameApp &operator=(GameApp &&) = delete;

private:
    [[nodiscard]] bool init();
    void handleEvent();
    void update(float delta_time);
    void render();
    void close();

    [[nodiscard]] bool initSDL();
    [[nodiscard]] bool initTime();
    [[nodiscard]] bool initResourceManager();
    [[nodiscard]] bool initRenderer();
    [[nodiscard]] bool initCamera();

    void testResourceManager();
    void testRenderer();
    void testCamera();
};
} // namespace engine::core
