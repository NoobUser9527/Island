#include "TextureManager.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>

namespace engine::resource
{
TextureManager::TextureManager(SDL_Renderer *renderer) : renderer_(renderer)
{
    if (renderer_ == nullptr)
    {
        throw std::runtime_error("TextureManager 构造失败: renderer 为空");
    }

    spdlog::trace("TextureManager 构造完成");
}

SDL_Texture *TextureManager::load(const std::string &file_path)
{
    if (texture_map_.find(file_path) != texture_map_.end())
    {
        spdlog::warn("TextureManager: 纹理已加载: {}", file_path);
        return texture_map_[file_path].get();
    }

    SDL_Texture *texture = IMG_LoadTexture(renderer_, file_path.c_str());
    if (texture == nullptr)
    {
        spdlog::error("TextureManager: 无法加载纹理: {}. SDL错误: {}", file_path, SDL_GetError());
        return nullptr;
    }

    texture_map_.emplace(file_path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(texture));
    spdlog::info("TextureManager: 纹理加载成功: {}", file_path);
    return texture;
}

SDL_Texture *TextureManager::get(const std::string &file_path)
{
    auto it = texture_map_.find(file_path);
    if (it != texture_map_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("TextureManager: 纹理未找到: {} 尝试加载", file_path);
        return load(file_path);
    }
}

glm::vec2 TextureManager::getSize(const std::string &file_path)
{
    SDL_Texture *texture = get(file_path);
    if (texture == nullptr)
    {
        spdlog::error("TextureManager: 无法获取纹理大小, 纹理未加载: {}", file_path);
        return glm::vec2(0.0f, 0.0f);
    }

    glm::vec2 size;
    if (!SDL_GetTextureSize(texture, &size.x, &size.y))
    {
        spdlog::error("TextureManager: 无法查询纹理大小: {}. SDL错误: {}", file_path, SDL_GetError());
        return glm::vec2(0.0f, 0.0f);
    }
    return size;
}

void TextureManager::unload(const std::string &file_path)
{
    auto it = texture_map_.find(file_path);
    if (it != texture_map_.end())
    {
        texture_map_.erase(it);
        spdlog::info("TextureManager: 纹理卸载成功: {}", file_path);
    }
    else
    {
        spdlog::warn("TextureManager: 纹理未找到, 无法卸载: {}", file_path);
    }
}

void TextureManager::clear()
{
    texture_map_.clear();
    spdlog::info("TextureManager: 所有纹理已卸载");
}

} // namespace engine::resource
