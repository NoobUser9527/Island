#pragma once
#include <SDL3/SDL_render.h>
#include <glm/vec2.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace engine::core
{
class Config;
}

namespace engine::input
{

enum class ActionState
{
    INACTIVE,
    PRESSED_THIS_FRAME,
    HELD_DOWN,
    RELEASED_THIS_FRAME
};

class InputManager final
{
private:
    SDL_Renderer* sdl_renderer_;
    std::unordered_map<std::string, std::vector<std::string>> actions_to_keyname_map_;
    std::unordered_map<std::variant<SDL_Scancode, Uint8>, std::vector<std::string>> input_to_actions_map_;

    std::unordered_map<std::string, ActionState> action_states_;

    bool should_quit_ = false;
    glm::vec2 mouse_position_;

public:
    InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config);

    void update();

    // 动作状态检查
    bool isActionDown(std::string_view action_name) const;
    bool isActionPressed(std::string_view action_name) const;
    bool isActionReleased(std::string_view action_name) const;

    bool shouldQuit() const;
    void setShouldQuit(bool should_quit);

    glm::vec2 getMousePosition() const;
    glm::vec2 getLogicalMousePosition() const;

private:
    void processEvent(const SDL_Event& event);
    void initializeMappings(const engine::core::Config* config);

    void updateActionState(std::string_view action_name, bool is_input_active, bool is_repeat_event);
    SDL_Scancode scancodeFromString(std::string_view key_name);
    Uint32 mouseButtonFromString(std::string_view button_name);
};

} // namespace engine::input
