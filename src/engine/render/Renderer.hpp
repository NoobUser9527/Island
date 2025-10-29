#pragma once

#include <optional>

#include <SDL3/SDL_stdinc.h>

#include "engine/utils/Math.hpp"

struct SDL_Renderer;
struct SDL_FRect;
struct SDL_FColor;

namespace engine::resource
{
class ResourceManager;
}

namespace engine::render
{

class Camera;
class Sprite;

class Renderer final
{
private:
    SDL_Renderer* renderer_ = nullptr;
    engine::resource::ResourceManager* resource_manager_ = nullptr;

public:
    Renderer(SDL_Renderer* renderer, engine::resource::ResourceManager* resourceManager);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale = {1.0f, 1.0f}, float angle = 0.0f);

    void drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, const glm::bvec2 repeat = {true, true}, const glm::vec2& scale = {1.0f, 1.0f});

    void drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size = std::nullopt);

    void present();
    void clearScreen();

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void setDrawColorFloat(float r, float g, float b, float a = 1.0f);

    SDL_Renderer* getSDLRenderer() const { return renderer_; }

private:
    std::optional<SDL_FRect> getSpritesRect(const Sprite& sprite);
    bool isRectInViewPort(const Camera& camera, const SDL_FRect& rect);
};
} // namespace engine::render
