#include "Config.hpp"

#include <exception>
#include <fstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace engine::core
{

Config::Config(const std::string& filepath)
{
    loadFromFile(filepath);
}

bool Config::loadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        spdlog::warn("Failed to open config file {}", filepath);
        if (!saveToFile(filepath))
        {
            spdlog::error("Failed to create default config file {}", filepath);
        }
        else
        {
            spdlog::trace("Created default config file {}", filepath);
        }
        return false;
    }

    try
    {
        nlohmann::json j;
        file >> j;
        fromJson(j);
        spdlog::info("Successfully read config file {}", filepath);
        return true;
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to read config file {}, using default config. Error: {}", filepath, e.what());
    }
    return false;
}

bool Config::saveToFile(const std::string& filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open())
    {
        spdlog::error("Failed to open config file {}, save aborted", filepath);
        return false;
    }

    try
    {
        nlohmann::ordered_json j = toJson();
        file << j.dump(4);
        spdlog::info("Successfully saved config to file {}", filepath);
        return true;
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to save config to file {}. Error: {}", filepath, e.what());
    }
    return false;
}

void Config::fromJson(const nlohmann::json& j)
{
    if (j.contains("window"))
    {
        const auto& window_config = j["window"];
        window_title_ = window_config.value("title", window_title_);
        window_width_ = window_config.value("width", window_width_);
        window_height_ = window_config.value("height", window_height_);
        window_resizeable_ = window_config.value("resizeable", window_resizeable_);
    }

    if (j.contains("graphics"))
    {
        const auto& graphics_config = j["graphics"];
        vsync_enabled_ = graphics_config.value("vsync", vsync_enabled_);
    }

    if (j.contains("performance"))
    {
        const auto& perf_config = j["performance"];
        target_fps_ = perf_config.value("target_fps", target_fps_);
        if (target_fps_ < 0)
        {
            spdlog::warn("Config target_fps is negative, resetting to 0 (no limit)");
            target_fps_ = 0;
        }
    }

    if (j.contains("audio"))
    {
        const auto& audio_config = j["audio"];
        music_volume_ = audio_config.value("music_volume", music_volume_);
        sound_volume_ = audio_config.value("sound_volume", sound_volume_);
    }

    if (j.contains("input_mappings") && j["input_mappings"].is_object())
    {
        const auto& mappings_config = j["input_mappings"];
        try
        {
            auto input_mappings = mappings_config.get<std::unordered_map<std::string, std::vector<std::string>>>();
            input_mappings_ = std::move(mappings_config);
            spdlog::trace("Loaded input mappings successfully");
        }
        catch (const std::exception& e)
        {
            spdlog::warn("Config file parsed but input mappings failed to load. Error: {}", e.what());
        }
    }
    else
    {
        spdlog::trace("No valid \"input_mappings\" found, using default mappings");
    }
}

nlohmann::ordered_json Config::toJson() const
{
    return nlohmann::ordered_json{
        {
            "window",
            {
                {"title", window_title_},
                {"width", window_width_},
                {"height", window_height_},
                {"resizeable", window_resizeable_},
            },
        },
        {
            "graphics",
            {
                {"vsync", vsync_enabled_},
            },
        },
        {
            "performance",
            {
                {"target_fps", target_fps_},
            },
        },
        {
            "audio",
            {
                {"music_volume", music_volume_},
                {"sound_volume", sound_volume_},
            },
        },
        {"input_mappings", input_mappings_},
    };
}

} // namespace engine::core
