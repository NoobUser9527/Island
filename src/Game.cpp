#include "Game.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using json = nlohmann::json;

Game::Game() {}
Game::~Game() {}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        spdlog::error("SDL init failed: {}", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("My SDL3 Game", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        spdlog::error("Failed to create window: {}", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        spdlog::error("Failed to create renderer: {}", SDL_GetError());
        return false;
    }

    // 例子：使用 glm 计算矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 2.0f, 3.0f));
    spdlog::info("Model matrix[0][0] = {}", model[0][0]);

    // 例子：加载 json 配置
    json config = {
        {"player", {{"hp", 100}, {"pos", {0,0,0}}}}
    };
    spdlog::info("Config: {}", config.dump());

    return true;
}

void Game::run() {
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // TODO: 游戏绘制逻辑

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void Game::shutdown() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
