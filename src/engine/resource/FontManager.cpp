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
        throw std::runtime_error("FontManager 构造失败: 无法初始化 SDL_ttf 原因：" + std::string(SDL_GetError()));
    }
    spdlog::trace("SDL_ttf 初始化成功");
    spdlog::trace("FontManager 构造完成");
}

FontManager::~FontManager()
{
    clear();
    if (TTF_WasInit())
    {
        TTF_Quit();
        spdlog::trace("SDL_ttf 已退出");
    }
    spdlog::trace("FontManager 析构完成");
}

TTF_Font *FontManager::load(const std::string &file_path, int font_size)
{
    if (file_path.empty() || font_size <= 0)
    {
        spdlog::error("FontManager::load: 无效的参数，路径: {}, 大小: {}", file_path, font_size);
        return nullptr;
    }
    FontKey key = std::make_pair(file_path, font_size);
    auto it = font_map_.find(key);
    if (it != font_map_.end())
    {
        spdlog::warn("FontManager::load: 字体已存在，路径: {}, 大小: {}", file_path, font_size);
        return it->second.get();
    }

    TTF_Font *font = TTF_OpenFont(file_path.c_str(), font_size);
    if (!font)
    {
        spdlog::error("FontManager::load: 无法加载字体，路径: {}, 大小: {}, 原因: {}", file_path, font_size,
                      SDL_GetError());
        return nullptr;
    }

    font_map_.emplace(key, std::unique_ptr<TTF_Font, SDLFontDeleter>(font));
    spdlog::trace("FontManager::load: 成功加载字体，路径: {}, 大小: {}", file_path, font_size);
    return font;
}

TTF_Font *FontManager::get(const std::string &file_path, int font_size)
{
    if (file_path.empty() || font_size <= 0)
    {
        spdlog::error("FontManager::get: 无效的参数，路径: {}, 大小: {}", file_path, font_size);
        return nullptr;
    }
    FontKey key = std::make_pair(file_path, font_size);
    auto it = font_map_.find(key);
    if (it != font_map_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("FontManager::get: 字体未找到，路径: {}, 大小: {}", file_path, font_size);
        return nullptr;
    }
}

void FontManager::unload(const std::string &file_path, int font_size)
{
    if (file_path.empty() || font_size <= 0)
    {
        spdlog::error("FontManager::unload: 无效的参数，路径: {}, 大小: {}", file_path, font_size);
        return;
    }
    FontKey key = std::make_pair(file_path, font_size);
    auto it = font_map_.find(key);
    if (it != font_map_.end())
    {
        font_map_.erase(it);
        spdlog::trace("FontManager::unload: 成功卸载字体，路径: {}, 大小: {}", file_path, font_size);
    }
    else
    {
        spdlog::warn("FontManager::unload: 字体未找到，路径: {}, 大小: {}", file_path, font_size);
    }
}

void FontManager::clear()
{
    if (!font_map_.empty())
    {
        font_map_.clear();
        spdlog::trace("FontManager::clear: 已卸载所有字体");
    }
}

} // namespace engine::resource