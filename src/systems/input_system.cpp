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
    const std::unordered_set<InputCode, InputCodeHash> action_inputs(
        action_inputs_list.begin(), action_inputs_list.end());
    _actions[action_name] = action_inputs;
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

std::optional<std::unordered_set<InputCode, InputCodeHash>>
InputSystem::get_inputs_from_action(const std::string &name) const
{
    // TODO: Make so this does not copy inputs everytime.
    if (const auto it = _actions.find(name); it != _actions.end())
        return it->second;
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
    auto inputs = get_inputs_from_action(action_name);
    return inputs.has_value() &&
           std::ranges::any_of(*inputs,
               [&](const InputCode& input) { return is_input_down(input); });
}

bool InputSystem::is_action_just_pressed(const std::string &action_name) const
{
    if (auto inputs = get_inputs_from_action(action_name)) {
        return std::ranges::any_of(*inputs,
           [&](const InputCode& input) {
               return is_input_down(input) &&
                      was_input_up(input);
           });
    }
    return false;
}

bool InputSystem::is_action_just_released(const std::string &action_name) const
{
    if (auto inputs = get_inputs_from_action(action_name)) {
        return std::ranges::any_of(*inputs,
           [&](const InputCode& input) {
               return is_input_up(input) &&
                      was_input_down(input);
           });
    }
    return false;
}