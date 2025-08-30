#include "GameApp.hpp"

#include <memory>

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>

#include "Time.hpp"
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
        spdlog::warn("GameApp 被销毁时没有显示关闭。现在关闭。 ...");
        close();
    }
}

void GameApp::run()
{
    if (!init())
    {
        spdlog::error("初始化失败，无法运行游戏");
        return;
    }

    time_->setTargetFps(60);

    while (is_running_)
    {
        time_->update();
        float delta_time = time_->getDeltaTime();
        handleEvent();
        update(delta_time);
        render();

        spdlog::info("delta_time: {}", delta_time);
    }

    close();
}

bool GameApp::init()
{
    spdlog::trace("初始化 GameAPP ...");
    if (!initSDL())
        return false;
    if (!initTime())
        return false;
    if (!initResourceManager())
        return false;

    testResourceManager();

    is_running_ = true;
    spdlog::trace("GameAPP 初始化完成");
    return true;
}

void GameApp::handleEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            is_running_ = false;
        }
    }
}
void GameApp::update(float delta_time)
{
    //
}

void GameApp::render()
{
    //
}

void GameApp::close()
{
    spdlog::trace("关闭 GameApp");
    if (sdl_renderer_ != nullptr)
    {
        SDL_DestroyRenderer(sdl_renderer_);
        sdl_renderer_ = nullptr;
    }
    if (window_ != nullptr)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
    is_running_ = false;
    spdlog::trace("GameApp 已关闭");
}

bool GameApp::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::error("SDL 初始化失败! SDL错误: {}", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow("Island", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (window_ == nullptr)
    {
        spdlog::error("无法创建窗口! SDL错误: {}", SDL_GetError());
        return false;
    }

    sdl_renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (sdl_renderer_ == nullptr)
    {
        spdlog::error("无法创建渲染器! SDL错误: {}", SDL_GetError());
        return false;
    }
    spdlog::trace("SDL 初始化成功");
    return true;
}

bool GameApp::initTime()
{
    try
    {
        time_ = std::make_unique<Time>();
    }
    catch (const std::exception &e)
    {
        spdlog::error("时间管理初始化失败: {}", e.what());
        return false;
    }
    spdlog::trace("时间管理初始化成功");
    return true;
}
bool GameApp::initResourceManager()
{
    try
    {
        resource_manager_ = std::make_unique<engine::resource::ResourceManager>(sdl_renderer_);
    }
    catch (const std::exception &e)
    {
        spdlog::error("资源管理初始化失败: {}", e.what());
        return false;
    }
    spdlog::trace("资源管理初始化成功");
    return true;
}
void GameApp::testResourceManager()
{
    if (!resource_manager_)
    {
        spdlog::error("资源管理器未初始化，无法测试");
        return;
    }

    // 测试纹理加载
    const std::string texture_path = ASSET_DIR "/textures/tileset.png";
    SDL_Texture *texture = resource_manager_->loadTexture(texture_path);
    if (texture)
    {
        spdlog::info("成功加载纹理: {}", texture_path);
        glm::vec2 size = resource_manager_->getTextureSize(texture_path);
        spdlog::info("纹理大小: {}x{}", size.x, size.y);
    }
    else
    {
        spdlog::error("加载纹理失败: {}", texture_path);
    }

    // 测试音效加载
    const std::string sound_path = ASSET_DIR "/sounds/jump.wav";
    MIX_Audio *sound = resource_manager_->loadSound(sound_path);
    if (sound)
    {
        spdlog::info("成功加载音效: {}", sound_path);
    }
    else
    {
        spdlog::error("加载音效失败: {}", sound_path);
    }

    // 测试音乐加载
    const std::string music_path = ASSET_DIR "/music/background.mp3";
    MIX_Audio *music = resource_manager_->loadMusic(music_path);
    if (music)
    {
        spdlog::info("成功加载音乐: {}", music_path);
    }
    else
    {
        spdlog::error("加载音乐失败: {}", music_path);
    }

    // 测试字体加载
    const std::string font_path = ASSET_DIR "/fonts/VonwaonBitmap-16px.ttf";
    int font_size = 24;
    TTF_Font *font = resource_manager_->loadFont(font_path, font_size);
    if (font)
    {
        spdlog::info("成功加载字体: {} 大小: {}", font_path, font_size);
    }
    else
    {
        spdlog::error("加载字体失败: {} 大小: {}", font_path, font_size);
    }
}

} // namespace engine::core
