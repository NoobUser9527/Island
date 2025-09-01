#include "TextureManager.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>

namespace engine::resource
{
TextureManager::TextureManager(SDL_Renderer* renderer)
    : renderer_(renderer)
{
    if (renderer_ == nullptr)
    {
        throw std::runtime_error("TextureManager construction failed: renderer is null");
    }

    spdlog::trace("TextureManager constructed successfully");
}

SDL_Texture* TextureManager::load(std::string_view file_path)
{
    if (auto it = texture_map_.find(std::string(file_path)); it != texture_map_.end())
    {
        spdlog::warn("TextureManager: texture already loaded: {}", file_path);
        return it->second.get();
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer_, file_path.data());
    if (texture == nullptr)
    {
        spdlog::error("TextureManager: failed to load texture: {}. SDL error: {}", file_path, SDL_GetError());
        return nullptr;
    }

    texture_map_.emplace(file_path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(texture));
    spdlog::info("TextureManager: texture loaded successfully: {}", file_path);
    return texture;
}

SDL_Texture* TextureManager::get(std::string_view file_path)
{
    if (auto it = texture_map_.find(std::string(file_path)); it != texture_map_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("TextureManager: texture not found: {}. Attempting to load...", file_path);
        return load(file_path);
    }
}

glm::vec2 TextureManager::getSize(std::string_view file_path)
{
    SDL_Texture* texture = get(file_path);
    if (texture == nullptr)
    {
        spdlog::error("TextureManager: cannot get size, texture not loaded: {}", file_path);
        return glm::vec2(0.0f, 0.0f);
    }

    glm::vec2 size;
    if (!SDL_GetTextureSize(texture, &size.x, &size.y))
    {
        spdlog::error("TextureManager: failed to query texture size: {}. SDL error: {}", file_path, SDL_GetError());
        return glm::vec2(0.0f, 0.0f);
    }
    return size;
}

void TextureManager::unload(std::string_view file_path)
{
    if (auto it = texture_map_.find(std::string(file_path)); it != texture_map_.end())
    {
        texture_map_.erase(it);
        spdlog::info("TextureManager: texture unloaded successfully: {}", file_path);
    }
    else
    {
        spdlog::warn("TextureManager: texture not found, cannot unload: {}", file_path);
    }
}

void TextureManager::clear()
{
    texture_map_.clear();
    spdlog::info("TextureManager: all textures have been unloaded");
}

} // namespace engine::resource
