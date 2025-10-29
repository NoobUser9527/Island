#include "GameApp.hpp"

#include <memory>
#include <vector>

#include <spdlog/spdlog.h>
#include <glm/vec2.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>

#include "engine/core/Config.hpp"
#include "engine/core/Time.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/render/Camera.hpp"
#include "engine/render/Renderer.hpp"
#include "engine/render/Sprite.hpp"
#include "engine/resource/ResourceManager.hpp"

namespace engine::core
{

GameApp::GameApp()
{
    time_ = std::make_unique<Time>();
}

GameApp::~GameApp()
{
    if (is_running_)
    {
        spdlog::warn("GameApp was not closed properly, closing now...");
        close();
    }
}

void GameApp::run()
{
    if (!init())
        return;

    while (is_running_)
    {
        time_->update();
        float delta_time = time_->getDeltaTime();
        input_manager_->update();

        handleEvent();
        update(delta_time);
        render();

        // spdlog::trace("delta_time: {}", delta_time);
    }
    close();
}

bool GameApp::init()
{
    spdlog::trace("Initializing GameApp...");
    if (!initConfig())
        return false;
    if (!initSDL())
        return false;
    if (!initTime())
        return false;
    if (!initResourceManager())
        return false;
    if (!initRenderer())
        return false;
    if (!initCamera())
        return false;
    if (!initInputManager())
        return false;

    testResourceManager();

    is_running_ = true;
    spdlog::info("Initialized GameApp");
    return true;
}

void GameApp::handleEvent()
{
    if (input_manager_->shouldQuit())
    {
        spdlog::trace("GameApp recv quit request from InputManager.");
        is_running_ = false;
        return;
    }

    testInputManager();
}

void GameApp::update(float delta_time)
{
    // game logic update
    testCamera();
}

void GameApp::render()
{
    renderer_->clearScreen();

    testRenderer();

    renderer_->present();
}

void GameApp::close()
{
    if (!is_running_)
    {
        spdlog::warn("GameApp::close() called but already closed");
        return;
    }

    spdlog::trace("Closing GameApp...");

    SDL_DestroyRenderer(sdl_renderer_);
    sdl_renderer_ = nullptr;

    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_Quit();
    is_running_ = false;

    spdlog::info("GameApp closed");
}

bool GameApp::initConfig()
{
    spdlog::trace("Initializing Config...");
    try
    {
        config_ = std::make_unique<engine::core::Config>(SOURCE_DIR "assets/config.json");
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to initialize Config: {}", e.what());
        return false;
    }
    spdlog::info("Initialized Config");
    return true;
}

bool GameApp::initSDL()
{
    spdlog::trace("Initializing SDL...");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != true)
    {
        spdlog::error("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    spdlog::trace("Creating window...");
    window_ = SDL_CreateWindow(config_->window_title_.c_str(), config_->window_width_, config_->window_height_, SDL_WINDOW_RESIZABLE);
    if (window_ == nullptr)
    {
        spdlog::error("Failed to create window: {}", SDL_GetError());
        return false;
    }
    spdlog::info("Created window: Island (1280x720)");

    spdlog::trace("Creating renderer...");
    sdl_renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (sdl_renderer_ == nullptr)
    {
        spdlog::error("Failed to create renderer: {}", SDL_GetError());
        return false;
    }

    int vsync_mode = config_->vsync_enabled_ ? SDL_RENDERER_VSYNC_ADAPTIVE : SDL_RENDERER_VSYNC_DISABLED;
    SDL_SetRenderVSync(sdl_renderer_, vsync_mode);
    spdlog::trace("VSyne set to: {}", config_->vsync_enabled_ ? "Enabled" : "Disabled");

    SDL_SetRenderLogicalPresentation(sdl_renderer_, config_->window_width_ / 2, config_->window_height_ / 2, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    spdlog::info("Created renderer");
    return true;
}

bool GameApp::initTime()
{
    spdlog::trace("Initializing Time...");
    try
    {
        time_ = std::make_unique<Time>();
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to initialize Time: {}", e.what());
        return false;
    }
    time_->setTargetFps(config_->target_fps_);
    spdlog::info("Initialized Time");
    return true;
}

bool GameApp::initResourceManager()
{
    spdlog::trace("Initializing ResourceManager...");
    try
    {
        resource_manager_ = std::make_unique<engine::resource::ResourceManager>(sdl_renderer_);
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to initialize ResourceManager: {}", e.what());
        return false;
    }
    spdlog::info("Initialized ResourceManager");
    return true;
}

bool GameApp::initRenderer()
{
    try
    {
        renderer_ = std::make_unique<engine::render::Renderer>(sdl_renderer_, resource_manager_.get());
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to initialize Renderer: {}", e.what());
        return false;
    }
    spdlog::info("Initialized Renderer");
    return true;
}

bool GameApp::initCamera()
{
    try
    {
        camera_ = std::make_unique<engine::render::Camera>(glm::vec2{config_->window_width_ / 2, config_->window_height_ / 2});
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to initialize Camera: {}", e.what());
        return false;
    }
    spdlog::info("Initialized Camera");
    return true;
}

bool GameApp::initInputManager()
{
    try
    {
        input_manager_ = std::make_unique<engine::input::InputManager>(sdl_renderer_, config_.get());
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to initialize InputManager: {}", e.what());
        return false;
    }
    spdlog::info("Initialized InputManager");
    return true;
}

void GameApp::testResourceManager()
{
    if (!resource_manager_)
    {
        spdlog::error("ResourceManager not initialized");
        return;
    }

    // Test texture loading
    const std::string texture_path = SOURCE_DIR "assets/textures/Layers/tileset.png";
    SDL_Texture* texture = resource_manager_->loadTexture(texture_path);
    if (texture)
    {
        spdlog::info("Loaded texture: {}", texture_path);
        glm::vec2 size = resource_manager_->getTextureSize(texture_path);
        spdlog::info("Texture {} size: {}x{}", texture_path, size.x, size.y);
    }
    else
    {
        spdlog::error("Failed to load texture: {}", texture_path);
    }

    // Test sound loading
    const std::string sound_path = SOURCE_DIR "assets/audio/monster.mp3";
    MIX_Audio* sound = resource_manager_->loadSound(sound_path);
    if (sound)
    {
        spdlog::info("Loaded sound: {}", sound_path);
    }
    else
    {
        spdlog::error("Failed to load sound: {}", sound_path);
    }

    // Test music loading
    const std::string music_path = SOURCE_DIR "assets/audio/poka01.mp3";
    MIX_Audio* music = resource_manager_->loadMusic(music_path);
    if (music)
    {
        spdlog::info("Loaded music: {}", music_path);
    }
    else
    {
        spdlog::error("Failed to load music: {}", music_path);
    }

    // Test font loading
    const std::string font_path = SOURCE_DIR "assets/fonts/VonwaonBitmap-16px.ttf";
    int font_size = 24;
    TTF_Font* font = resource_manager_->loadFont(font_path, font_size);
    if (font)
    {
        spdlog::info("Loaded font: {} size: {}", font_path, font_size);
    }
    else
    {
        spdlog::error("Failed to load font: {} size: {}", font_path, font_size);
    }
}

void GameApp::testRenderer()
{
    engine::render::Sprite sprite_world(SOURCE_DIR "assets/textures/Actors/frog.png");
    engine::render::Sprite sprite_ui(SOURCE_DIR "assets/textures/UI/buttons/Start1.png");
    engine::render::Sprite sprite_parallad(SOURCE_DIR "assets/textures/Layers/back.png");

    static float rotation = 0.0f;
    rotation += 0.1f;

    renderer_->drawParallax(*camera_, sprite_parallad, glm::vec2(100.0f, 100.0f), glm::vec2(0.5f, 0.5f), glm::bvec2(true, false));
    renderer_->drawSprite(*camera_, sprite_world, glm::vec2(200.0f, 200.0f), glm::vec2(1.0f, 1.0f), rotation);
    renderer_->drawUISprite(sprite_ui, glm::vec2(100.0f, 100.0f));
}

void GameApp::testCamera()
{
    auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_UP])
        camera_->move(glm::vec2(0.0f, -1.0f));
    if (key_state[SDL_SCANCODE_DOWN])
        camera_->move(glm::vec2(0.0f, 1.0f));
    if (key_state[SDL_SCANCODE_LEFT])
        camera_->move(glm::vec2(-1.0f, 0.0f));
    if (key_state[SDL_SCANCODE_RIGHT])
        camera_->move(glm::vec2(1.0f, 0.0f));
}

void GameApp::testInputManager()
{
    std::vector<std::string> actions = {
        "move_up",
        "move_down",
        "move_left",
        "move_right",
        "jump",
        "attack",
        "pause",
        "MouseLeftClick",
        "MouseRightClick",
    };

    for (const auto& action : actions)
    {
        if (input_manager_->isActionPressed(action))
        {
            spdlog::info("{} Pressed", action);
        }
        if (input_manager_->isActionReleased(action))
        {
            spdlog::info("{} Released", action);
        }
        if (input_manager_->isActionDown(action))
        {
            spdlog::info("{} Down", action);
        }
    }
}

} // namespace engine::core
