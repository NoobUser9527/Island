#pragma once

#include <memory>
#include <unordered_map>

#include "glm/fwd.hpp"
#include <SDL3/SDL_render.h>

namespace engine::resource
{

class TextureManager final
{
    friend class ResourceManager;

private:
    struct SDLTextureDeleter
    {
        void operator()(SDL_Texture* texture) const
        {
            if (texture)
            {
                SDL_DestroyTexture(texture);
            }
        }
    };
    SDL_Renderer* renderer_ = nullptr;
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> texture_map_;

public:
    explicit TextureManager(SDL_Renderer* renderer);

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;

private:
    SDL_Texture* load(std::string_view file_path);
    SDL_Texture* get(std::string_view file_path);
    glm::vec2 getSize(std::string_view file_path);
    void unload(std::string_view file_path);
    void clear();
};

} // namespace engine::resource