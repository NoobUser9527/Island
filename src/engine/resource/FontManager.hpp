#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace engine::resource {

class FontManager final {

    friend class ResourceManager;

  private:
    using FontKey = std::pair<std::string, int>;
    struct SDLFontDeleter {
        void operator()(TTF_Font *font) const {
            if (font) {
                TTF_CloseFont(font);
            }
        }
    };

    struct FontKeyHash {
        std::size_t operator()(const FontKey &key) const { return std::hash<std::string>()(key.first) ^ std::hash<int>()(key.second); }
    };
    std::unordered_map<FontKey, std::unique_ptr<TTF_Font, SDLFontDeleter>, FontKeyHash> font_map_;

  public:
    FontManager();
    ~FontManager();

    FontManager(const FontManager &) = delete;
    FontManager &operator=(const FontManager &) = delete;
    FontManager(FontManager &&) = delete;
    FontManager &operator=(FontManager &&) = delete;

  private:
    TTF_Font *load(const std::string &file_path, int font_size);
    TTF_Font *get(const std::string &file_path, int font_size);
    void unload(const std::string &file_path, int font_size);
    void clear();
};
} // namespace engine::resource