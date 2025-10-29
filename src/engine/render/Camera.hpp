#pragma once

#include "engine/utils/Math.hpp"
#include <optional>

namespace engine::render
{

class Camera final
{
private:
    glm::vec2 viewport_size_;
    glm::vec2 position_;
    std::optional<engine::utils::Rect> limit_bounds_;

public:
    Camera(const glm::vec2& viewport_size, const glm::vec2& position_ = glm::vec2(0.0f, 0.0f), const std::optional<engine::utils::Rect> limit_bounds = std::nullopt);

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

    void update(float delta_time);
    void move(const glm::vec2& offset);

    glm::vec2 worldToScreen(const glm::vec2& world_pos) const;
    glm::vec2 worldToScreenWitchParallax(const glm::vec2& world_pos, const glm::vec2& scroll_factor) const;
    glm::vec2 screenToWorld(const glm::vec2& screen_pos) const;

    void setPosition(const glm::vec2& position);
    void setLimitBounds(const std::optional<engine::utils::Rect>& limit_bounds);

    const glm::vec2& getViewportSize() const;
    const glm::vec2& getPosition() const;
    const std::optional<engine::utils::Rect>& getLimitBounds() const;

private:
    void clampPosition();
};
} // namespace engine::render
