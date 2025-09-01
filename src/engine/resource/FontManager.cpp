#include "FontManager.hpp"
#include "SDL3_ttf/SDL_ttf.h"
#include <string>

#include <spdlog/spdlog.h>

namespace engine::resource
{

FontManager::FontManager()
{
    if (!TTF_WasInit() && !TTF_Init())
    {
        throw std::runtime_error("FontManager construction failed: Unable to initialize SDL_ttf. Reason: " + std::string(SDL_GetError()));
    }
    spdlog::trace("SDL_ttf initialized successfully");
    spdlog::trace("FontManager constructed");
}

FontManager::~FontManager()
{
    clear();
    if (TTF_WasInit())
    {
        TTF_Quit();
        spdlog::trace("SDL_ttf shut down");
    }
    spdlog::trace("FontManager destructed");
}

TTF_Font* FontManager::load(std::string_view file_path, int font_size)
{
    if (file_path.empty() || font_size <= 0)
    {
        spdlog::error("FontManager::load: Invalid arguments. Path: {}, Size: {}", file_path, font_size);
        return nullptr;
    }

    FontKey key = {std::string(file_path), font_size};

    if (auto it = font_map_.find(key); it != font_map_.end())
    {
        spdlog::warn("FontManager::load: Font already loaded. Path: {}, Size: {}", file_path, font_size);
        return it->second.get();
    }

    TTF_Font* font = TTF_OpenFont(file_path.data(), font_size);
    if (!font)
    {
        spdlog::error("FontManager::load: Failed to load font. Path: {}, Size: {}, Reason: {}", file_path, font_size, SDL_GetError());
        return nullptr;
    }

    font_map_.emplace(key, std::unique_ptr<TTF_Font, SDLFontDeleter>(font));
    spdlog::trace("FontManager::load: Successfully loaded font. Path: {}, Size: {}", file_path, font_size);
    return font;
}

TTF_Font* FontManager::get(std::string_view file_path, int font_size)
{
    if (file_path.empty() || font_size <= 0)
    {
        spdlog::error("FontManager::get: Invalid arguments. Path: {}, Size: {}", file_path, font_size);
        return nullptr;
    }

    FontKey key = {std::string(file_path), font_size};

    if (auto it = font_map_.find(key); it != font_map_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("FontManager::get: Font not found. Path: {}, Size: {}", file_path, font_size);
        return nullptr;
    }
}

void FontManager::unload(std::string_view file_path, int font_size)
{
    if (file_path.empty() || font_size <= 0)
    {
        spdlog::error("FontManager::unload: Invalid arguments. Path: {}, Size: {}", file_path, font_size);
        return;
    }

    FontKey key = {std::string(file_path), font_size};

    if (auto it = font_map_.find(key); it != font_map_.end())
    {
        font_map_.erase(it);
        spdlog::trace("FontManager::unload: Successfully unloaded font. Path: {}, Size: {}", file_path, font_size);
    }
    else
    {
        spdlog::warn("FontManager::unload: Font not found. Path: {}, Size: {}", file_path, font_size);
    }
}

void FontManager::clear()
{
    if (!font_map_.empty())
    {
        font_map_.clear();
        spdlog::trace("FontManager::clear: All fonts unloaded");
    }
}

} // namespace engine::resource
