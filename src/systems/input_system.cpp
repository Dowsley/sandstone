//
// Created by João Dowsley on 08/08/25.
//

#include "input_system.h"

#include <raylib.h>

void InputSystem::rebuild_watched_keys()
{
    _watched_keys.clear();
    for (const auto &keys : _actions | std::views::values) {
        _watched_keys.insert(keys.begin(), keys.end());
    }
}

bool InputSystem::is_key_down(const int key) const
{
    return _current_keyboard_state.contains(key);
}

bool InputSystem::is_key_up(const int key) const
{
    return !_current_keyboard_state.contains(key);
}

bool InputSystem::was_key_down(const int key) const
{
    return _previous_keyboard_state.contains(key);
}

bool InputSystem::was_key_up(const int key) const
{
    return !_previous_keyboard_state.contains(key);
}

void InputSystem::update()
{
    _previous_keyboard_state = std::move(_current_keyboard_state);
    _current_keyboard_state.clear();

    for (int key : _watched_keys) {
        if (IsKeyDown(key)) // the only polling we do from raylib
            _current_keyboard_state.insert(key);
    }
}

void InputSystem::create_action(const std::string &action_name,
    const std::list<int> &action_keys_list)
{
    const std::unordered_set action_keys(
        action_keys_list.begin(),action_keys_list.end());
    _actions[action_name] = action_keys;
    rebuild_watched_keys();
}

bool InputSystem::add_key_to_action(const std::string &action_name, const int action_key)
{
    if (!_actions.contains(action_name)) {
        return false;
    }
        
    _actions[action_name].insert(action_key);
    rebuild_watched_keys();
    return true;
}

std::optional<std::unordered_set<int>> InputSystem::get_keys_from_action(
    const std::string &name) const
{
    // TODO: Make so this does not copy keys everytime.
    if (const auto it = _actions.find(name); it != _actions.end())
        return it->second;
    return std::nullopt;
}

bool InputSystem::delete_action(const std::string &action_name)
{
    const bool result = _actions.erase(action_name);
    rebuild_watched_keys();
    return result;
}

bool InputSystem::has_action(const std::string &action_name) const
{
    return _actions.contains(action_name);
}

bool InputSystem::is_action_pressed(const std::string &action_name) const
{
    auto keys = get_keys_from_action(action_name);
    return keys.has_value() &&
           std::ranges::any_of(*keys,
               [&](const int key) { return is_key_down(key); });
}

bool InputSystem::is_action_just_pressed(const std::string &action_name) const
{
    if (auto keys = get_keys_from_action(action_name)) {
        return std::ranges::any_of(*keys,
           [&](const int key) {
               return is_key_down(key) &&
                      was_key_up(key);
           });
    }
    return false;
}

bool InputSystem::is_action_just_released(const std::string &action_name) const
{
    if (auto keys = get_keys_from_action(action_name)) {
        return std::ranges::any_of(*keys,
           [&](const int key) {
               return is_key_up(key) &&
                      was_key_down(key);
           });
    }
    return false;
}