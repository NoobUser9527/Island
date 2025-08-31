#pragma once

#include "SDL3/SDL_gpu.h"
#include <SDL3/SDL_rect.h>

#include <optional>
#include <string>
#include <string_view>

namespace engine::render
{

class Sprite final
{
private:
    std::string texture_id_;
    std::optional<SDL_FRect> source_rect_;
    bool is_flipped_ = false;

public:
    Sprite(std::string_view testure_id, const std::optional<SDL_FRect>& source_rect = std::nullopt, bool is_flipped = false)
        : texture_id_(testure_id)
        , source_rect_(source_rect)
        , is_flipped_(is_flipped)
    {
    }

    const std::string_view getTextureId() const { return texture_id_; }
    const std::optional<SDL_FRect>& getSourceRect() const { return source_rect_; }
    bool isFlipped() const { return is_flipped_; }

    void setTextureId(std::string_view texture_id) { texture_id_ = std::string(texture_id); }
    void setSourceRect(std::optional<SDL_FRect> source_rect) { source_rect_ = std::move(source_rect); }
    void setFlipped(bool is_flipped) { is_flipped_ = is_flipped; }
};

} // namespace engine::render