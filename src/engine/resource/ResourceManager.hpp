#pragma once

#include <memory>

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
    explicit ResourceManager(SDL_Renderer *renderer);
    ~ResourceManager();

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    void clear();
    //
    SDL_Texture *loadTexture(const std::string &file_path);
    SDL_Texture *getTexture(const std::string &file_path);
    void unloadTexture(const std::string &file_path);
    glm::vec2 getTextureSize(const std::string &file_path);
    void clearTexture();

    //
    MIX_Audio *loadSound(const std::string &file_path);
    MIX_Audio *getSound(const std::string &file_path);
    void unloadSound(const std::string &file_path);
    void clearSound();

    //
    MIX_Audio *loadMusic(const std::string &file_path);
    MIX_Audio *getMusic(const std::string &file_path);
    void unloadMusic(const std::string &file_path);
    void clearMusic();

    //
    TTF_Font *loadFont(const std::string &file_path, int font_size);
    TTF_Font *getMFont(const std::string &file_path, int font_size);
    void unloadFont(const std::string &file_path, int font_size);
    void clearFont();
};
} // namespace engine::resource