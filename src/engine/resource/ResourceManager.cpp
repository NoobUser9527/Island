#include "ResourceManager.hpp"

#include "AudioManager.hpp"
#include "FontManager.hpp"
#include "TextureManager.hpp"

#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>

namespace engine::resource
{

ResourceManager::ResourceManager(SDL_Renderer *renderer)
{
    texture_manager_ = std::make_unique<TextureManager>(renderer);
    audio_manager_ = std::make_unique<AudioManager>();
    font_manager_ = std::make_unique<FontManager>();

    spdlog::trace("ResourceManager initialized successfully");
}

ResourceManager::~ResourceManager()
{
    clear();
    spdlog::trace("ResourceManager destroyed successfully");
}

void ResourceManager::clear()
{
    texture_manager_->clear();
    audio_manager_->clear();
    font_manager_->clear();
}

SDL_Texture *ResourceManager::loadTexture(std::string_view file_path)
{
    return texture_manager_->load(file_path);
}

SDL_Texture *ResourceManager::getTexture(std::string_view file_path)
{
    return texture_manager_->get(file_path);
}

void ResourceManager::unloadTexture(std::string_view file_path)
{
    texture_manager_->unload(file_path);
}

glm::vec2 ResourceManager::getTextureSize(std::string_view file_path)
{
    return texture_manager_->getSize(file_path);
}

void ResourceManager::clearTexture()
{
    texture_manager_->clear();
}

MIX_Audio *ResourceManager::loadSound(std::string_view file_path)
{
    return audio_manager_->load(file_path);
}
MIX_Audio *ResourceManager::getSound(std::string_view file_path)
{
    return audio_manager_->get(file_path);
}
void ResourceManager::unloadSound(std::string_view file_path)
{
    audio_manager_->unload(file_path);
}
void ResourceManager::clearSound()
{
    audio_manager_->clear();
}
MIX_Audio *ResourceManager::loadMusic(std::string_view file_path)
{
    return audio_manager_->load(file_path);
}
MIX_Audio *ResourceManager::getMusic(std::string_view file_path)
{
    return audio_manager_->get(file_path);
}
void ResourceManager::unloadMusic(std::string_view file_path)
{
    audio_manager_->unload(file_path);
}
void ResourceManager::clearMusic()
{
    audio_manager_->clear();
}
TTF_Font *ResourceManager::loadFont(std::string_view file_path, int font_size)
{
    return font_manager_->load(file_path, font_size);
}
TTF_Font *ResourceManager::getMFont(std::string_view file_path, int font_size)
{
    return font_manager_->get(file_path, font_size);
}

void ResourceManager::unloadFont(std::string_view file_path, int font_size)
{
    font_manager_->unload(file_path, font_size);
}

void ResourceManager::clearFont()
{
    font_manager_->clear();
}

} // namespace engine::resource