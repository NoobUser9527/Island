#pragma once

#include <memory>
#include <string_view>

#include <SDL3/SDL_render.h>
#include <glm/fwd.hpp>

struct SDL_Renderer;
struct SDL_Texture;
struct MIX_Audio;
struct TTF_Font;

namespace engine::resource
{

class TextureManager;
class AudioManager;
class FontManager;

class ResourceManager final
{
private:
    std::unique_ptr<TextureManager> texture_manager_;
    std::unique_ptr<AudioManager> audio_manager_;
    std::unique_ptr<FontManager> font_manager_;

public:
    explicit ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    void clear();
    //
    SDL_Texture* loadTexture(std::string_view file_path);
    SDL_Texture* getTexture(std::string_view file_path);
    void unloadTexture(std::string_view file_path);
    glm::vec2 getTextureSize(std::string_view file_path);
    void clearTexture();

    //
    MIX_Audio* loadSound(std::string_view file_path);
    MIX_Audio* getSound(std::string_view file_path);
    void unloadSound(std::string_view file_path);
    void clearSound();

    //
    MIX_Audio* loadMusic(std::string_view file_path);
    MIX_Audio* getMusic(std::string_view file_path);
    void unloadMusic(std::string_view file_path);
    void clearMusic();

    //
    TTF_Font* loadFont(std::string_view file_path, int font_size);
    TTF_Font* getMFont(std::string_view file_path, int font_size);
    void unloadFont(std::string_view file_path, int font_size);
    void clearFont();
};
} // namespace engine::resource