#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json_fwd.hpp>

namespace engine::core
{

class Config final
{

public:
    std::string window_title_ = "Island";
    int window_width_ = 1280;
    int window_height_ = 720;
    bool window_resizeable_ = true;

    bool vsync_enabled_ = true;
    int target_fps_ = 60;

    float music_volume_ = 0.5f;
    float sound_volume_ = 0.5f;

    std::unordered_map<std::string, std::vector<std::string>> input_mappings_ = {
        {"move_left", {"A", "Left"}},
        {"move_right", {"D", "Right"}},
        {"move_up", {"W", "Up"}},
        {"move_down", {"S", "Down"}},
        {"jump", {"K", "Space"}},
        {"attack", {"J", "MouseLeft"}},
        {"pause", {"P", "Escape"}},
    };

    explicit Config(const std::string& filepath);

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

    bool loadFromFile(const std::string& filepath);
    [[nodiscard]] bool saveToFile(const std::string& filepath);

private:
    void fromJson(const nlohmann::json& j);
    nlohmann::ordered_json toJson() const;
};

} // namespace engine::core