#include "Renderer.hpp"

#include <cstddef>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "engine/render/Camera.hpp"
#include "engine/render/Sprite.hpp"
#include "engine/resource/ResourceManager.hpp"

namespace engine::render
{

Renderer::Renderer(SDL_Renderer* renderer, engine::resource::ResourceManager* resourceManager)
    : renderer_(renderer)
    , resource_manager_(resourceManager)
{
    spdlog::trace("Constructing Renderer...");

    if (renderer_ == nullptr)
    {
        throw std::runtime_error("Failed to construct Renderer: input renderer is nullptr");
    }
    if (resource_manager_ == nullptr)
    {
        throw std::runtime_error("Failed to construct Renderer: input ResourceManager is nullptr");
    }

    setDrawColor(0, 0, 0, 255);
    spdlog::trace("Renderer constructed successfully");
}

void Renderer::drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float angle)
{
    auto texture = resource_manager_->getTexture(sprite.getTextureId());
    if (!texture)
    {
        spdlog::error("getTexture {} failed", sprite.getTextureId());
        return;
    }

    auto src_rect = getSpritesRect(sprite);
    if (!src_rect.has_value())
    {
        spdlog::error("get sprite {} rect failed", sprite.getTextureId());
        return;
    }

    glm::vec2 position_screen = camera.worldToScreen(position);

    float scaled_w = src_rect->w * scale.x;
    float scaled_h = src_rect->h * scale.y;
    SDL_FRect dest_rect = {position_screen.x, position_screen.y, scaled_w, scaled_h};

    if (!isRectInViewPort(camera, dest_rect))
        return;

    if (!SDL_RenderTextureRotated(renderer_, texture, &src_rect.value(), &dest_rect, angle, NULL, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
    {
        spdlog::error("SDL_RenderTextureRotated failed for texture {}. Error: {}", sprite.getTextureId(), SDL_GetError());
    }
}

void Renderer::drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, const glm::bvec2 repeat, const glm::vec2& scale)
{
    auto texture = resource_manager_->getTexture(sprite.getTextureId());
    if (!texture)
    {
        spdlog::error("getTexture {} failed", sprite.getTextureId());
        return;
    }

    auto src_rect = getSpritesRect(sprite);
    if (!src_rect.has_value())
    {
        spdlog::error("get sprite {} rect failed", sprite.getTextureId());
        return;
    }

    glm::vec2 position_screen = camera.worldToScreenWitchParallax(position, scroll_factor);

    float scaled_tex_w = src_rect->w * scale.x;
    float scaled_tex_h = src_rect->h * scale.y;

    glm::vec2 start, end;
    glm::vec2 viewport_size = camera.getViewportSize();

    if (repeat.x)
    {
        start.x = glm::mod(position_screen.x, scaled_tex_w) - scaled_tex_w;
        end.x = viewport_size.x;
    }
    else
    {
        start.x = position_screen.x;
        end.x = glm::min(position_screen.x + scaled_tex_w, viewport_size.x);
    }

    if (repeat.y)
    {
        start.y = glm::mod(position_screen.y, scaled_tex_h) - scaled_tex_h;
        end.y = viewport_size.y;
    }
    else
    {
        start.y = position_screen.y;
        end.y = glm::min(position_screen.y + scaled_tex_h, viewport_size.y);
    }

    for (float y = start.y; y < end.y; y += scaled_tex_h)
    {
        for (float x = start.x; x < end.x; x += scaled_tex_w)
        {
            SDL_FRect dest_rect = {x, y, scaled_tex_w, scaled_tex_h};
            if (!SDL_RenderTexture(renderer_, texture, nullptr, &dest_rect))
            {
                spdlog::error("SDL_RenderTexture failed for texture {}. Error: {}", sprite.getTextureId(), SDL_GetError());
                return;
            }
        }
    }
}

void Renderer::drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size)
{
    auto texture = resource_manager_->getTexture(sprite.getTextureId());
    if (!texture)
    {
        spdlog::error("getTexture {} failed", sprite.getTextureId());
        return;
    }

    auto src_rect = getSpritesRect(sprite);
    if (!src_rect.has_value())
    {
        spdlog::error("get sprite {} rect failed", sprite.getTextureId());
        return;
    }

    SDL_FRect dest_rect = {position.x, position.y, 0.0f, 0.0f};
    if (size.has_value())
    {
        dest_rect.w = size->x;
        dest_rect.h = size->y;
    }
    else
    {
        dest_rect.w = src_rect->w;
        dest_rect.h = src_rect->h;
    }

    if (!SDL_RenderTextureRotated(renderer_, texture, &src_rect.value(), &dest_rect, 0.0f, nullptr, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
    {
        spdlog::error("SDL_RenderTextureRotated failed for texture {}. Error: {}", sprite.getTextureId(), SDL_GetError());
    }
}

void Renderer::present()
{
    SDL_RenderPresent(renderer_);
}

void Renderer::clearScreen()
{
    if (!SDL_RenderClear(renderer_))
    {
        spdlog::error("SDL_RenderClear failed. Error: {}", SDL_GetError());
    }
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (!SDL_SetRenderDrawColor(renderer_, r, g, b, a))
    {
        spdlog::error("SDL_SetRenderDrawColor failed. Error: {}", SDL_GetError());
    }
}

void Renderer::setDrawColorFloat(float r, float g, float b, float a)
{
    if (!SDL_SetRenderDrawColorFloat(renderer_, r, g, b, a))
    {
        spdlog::error("SDL_SetRenderDrawColorFloat failed. Error: {}", SDL_GetError());
    }
}

std::optional<SDL_FRect> Renderer::getSpritesRect(const Sprite& sprite)
{
    SDL_Texture* texture = resource_manager_->getTexture(sprite.getTextureId());
    if (!texture)
    {
        spdlog::error("Failed to get texture with ID: {}", sprite.getTextureId());
        return std::nullopt;
    }

    auto src_rect = sprite.getSourceRect();
    if (src_rect.has_value())
    {
        if (src_rect->w <= 0 || src_rect->h <= 0)
        {
            spdlog::error("Texture {} source rect is invalid", sprite.getTextureId());
            return std::nullopt;
        }
        return src_rect;
    }
    else
    {
        SDL_FRect result = {0.0f, 0.0f, 0.0f, 0.0f};
        if (!SDL_GetTextureSize(texture, &result.w, &result.h))
        {
            spdlog::error("Failed to get size of texture {}", sprite.getTextureId());
            return std::nullopt;
        }
        return result;
    }
}

bool Renderer::isRectInViewPort(const Camera& camera, const SDL_FRect& rect)
{
    glm::vec2 viewport_size = camera.getViewportSize();
    return (rect.x + rect.w >= 0 && rect.x <= viewport_size.x) && (rect.y + rect.h >= 0 && rect.y <= viewport_size.y);
}

} // namespace engine::render
