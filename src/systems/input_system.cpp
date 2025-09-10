//
// Created by João Dowsley on 08/08/25.
//

#include "input_system.h"

#include <raylib.h>

void InputSystem::rebuild_watched_inputs()
{
    _watched_inputs.clear();
    for (const auto &inputs : _actions | std::views::values) {
        _watched_inputs.insert(inputs.begin(), inputs.end());
    }
}

bool InputSystem::is_input_down(const InputCode& input) const
{
    return _current_input_state.contains(input);
}

bool InputSystem::is_input_up(const InputCode& input) const
{
    return !_current_input_state.contains(input);
}

bool InputSystem::was_input_down(const InputCode& input) const
{
    return _previous_input_state.contains(input);
}

bool InputSystem::was_input_up(const InputCode& input) const
{
    return !_previous_input_state.contains(input);
}

void InputSystem::update()
{
    _previous_input_state = std::move(_current_input_state);
    _current_input_state.clear();

    // Update mouse state
    _previous_mouse_position = _current_mouse_position;
    _previous_mouse_wheel = _current_mouse_wheel;
    
    _current_mouse_position = GetMousePosition();
    _current_mouse_wheel = GetMouseWheelMove();

    for (const InputCode &input : _watched_inputs) {
        bool is_pressed = false;
        if (input.type == InputCode::KEY) {
            is_pressed = IsKeyDown(input.code);
        } else if (input.type == InputCode::MOUSE_BUTTON) {
            is_pressed = IsMouseButtonDown(input.code);
        }
        
        if (is_pressed) {
            _current_input_state.insert(input);
        }
    }
}

void InputSystem::create_action(const std::string &action_name,
    const std::list<InputCode> &action_inputs_list)
{
    InputState action_inputs(
        action_inputs_list.begin(), action_inputs_list.end());
    _actions[action_name] = std::move(action_inputs);
    rebuild_watched_inputs();
}

bool InputSystem::add_input_to_action(const std::string &action_name, const InputCode &action_input)
{
    if (!_actions.contains(action_name)) {
        return false;
    }
        
    _actions[action_name].insert(action_input);
    rebuild_watched_inputs();
    return true;
}

std::optional<std::reference_wrapper<const InputState>>
InputSystem::get_inputs_from_action(const std::string &name) const
{
    if (const auto it = _actions.find(name); it != _actions.end())
        return std::cref(it->second);
    return std::nullopt;
}

bool InputSystem::delete_action(const std::string &action_name)
{
    const bool result = _actions.erase(action_name);
    rebuild_watched_inputs();
    return result;
}

bool InputSystem::has_action(const std::string &action_name) const
{
    return _actions.contains(action_name);
}

bool InputSystem::is_action_pressed(const std::string &action_name) const
{
    const auto inputs = get_inputs_from_action(action_name);
    return inputs.has_value() &&
           std::ranges::any_of(inputs->get(),
               [&](const InputCode& input) { return is_input_down(input); });
}

bool InputSystem::is_action_just_pressed(const std::string &action_name) const
{
    if (const auto inputs = get_inputs_from_action(action_name)) {
        return std::ranges::any_of(inputs->get(),
           [&](const InputCode& input) {
               return is_input_down(input) &&
                      was_input_up(input);
           });
    }
    return false;
}

bool InputSystem::is_action_just_released(const std::string &action_name) const
{
    if (const auto inputs = get_inputs_from_action(action_name)) {
        return std::ranges::any_of(inputs->get(),
           [&](const InputCode& input) {
               return is_input_up(input) &&
                      was_input_down(input);
           });
    }
    return false;
}

Vector2 InputSystem::get_mouse_position() const
{
    return _current_mouse_position;
}

Vector2 InputSystem::get_mouse_delta() const
{
    return Vector2{
        _current_mouse_position.x - _previous_mouse_position.x,
        _current_mouse_position.y - _previous_mouse_position.y
    };
}

float InputSystem::get_mouse_scroll_delta() const
{
    // Raylib's GetMouseWheelMove() already returns the per-frame delta.
    return _current_mouse_wheel;
}