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

    spdlog::trace("ResourceManager 初始化完成");
}

ResourceManager::~ResourceManager()
{
    clear();
    spdlog::trace("ResourceManager 销毁完成");
}

void ResourceManager::clear()
{
    texture_manager_->clear();
    audio_manager_->clear();
    font_manager_->clear();
}

SDL_Texture *ResourceManager::loadTexture(const std::string &file_path)
{
    return texture_manager_->load(file_path);
}

SDL_Texture *ResourceManager::getTexture(const std::string &file_path)
{
    return texture_manager_->get(file_path);
}

void ResourceManager::unloadTexture(const std::string &file_path)
{
    texture_manager_->unload(file_path);
}

glm::vec2 ResourceManager::getTextureSize(const std::string &file_path)
{
    return texture_manager_->getSize(file_path);
}

void ResourceManager::clearTexture()
{
    texture_manager_->clear();
}

MIX_Audio *ResourceManager::loadSound(const std::string &file_path)
{
    return audio_manager_->load(file_path);
}
MIX_Audio *ResourceManager::getSound(const std::string &file_path)
{
    return audio_manager_->get(file_path);
}
void ResourceManager::unloadSound(const std::string &file_path)
{
    audio_manager_->unload(file_path);
}
void ResourceManager::clearSound()
{
    audio_manager_->clear();
}
MIX_Audio *ResourceManager::loadMusic(const std::string &file_path)
{
    return audio_manager_->load(file_path);
}
MIX_Audio *ResourceManager::getMusic(const std::string &file_path)
{
    return audio_manager_->get(file_path);
}
void ResourceManager::unloadMusic(const std::string &file_path)
{
    audio_manager_->unload(file_path);
}
void ResourceManager::clearMusic()
{
    audio_manager_->clear();
}
TTF_Font *ResourceManager::loadFont(const std::string &file_path, int font_size)
{
    return font_manager_->load(file_path, font_size);
}
TTF_Font *ResourceManager::getMFont(const std::string &file_path, int font_size)
{
    return font_manager_->get(file_path, font_size);
}

void ResourceManager::unloadFont(const std::string &file_path, int font_size)
{
    font_manager_->unload(file_path, font_size);
}

void ResourceManager::clearFont()
{
    font_manager_->clear();
}

} // namespace engine::resource