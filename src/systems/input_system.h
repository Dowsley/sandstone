//
// Created by João Dowsley on 08/08/25.
//

#ifndef SANDSTONE_INPUT_SYSTEM_H
#define SANDSTONE_INPUT_SYSTEM_H
#include <list>
#include <ranges>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <optional>

/**
 * @brief Handles inputs for RayLib in the same style as Godot's Input class, based on named actions.
 * 
 * @see https://www.raylib.com/cheatsheet/cheatsheet.html
 */
class InputSystem
{
private:
    // TODO: Add mouse input handling
    
    std::unordered_map<std::string, std::unordered_set<int>> _actions;
    std::unordered_set<int> _previous_keyboard_state;
    std::unordered_set<int> _current_keyboard_state;

    std::unordered_set<int> _watched_keys; // Cache of all keys referenced by any key action
    void rebuild_watched_keys();

    bool is_key_down(int key) const;

    bool is_key_up(int key) const;

    bool was_key_down(int key) const;

    bool was_key_up(int key) const;

public:
    /**
     * @brief Updates the internal keyboard and mouse states.
     * 
     * Must be called once per frame to enable edge detection (just pressed/released).
     */
    void update();

    /**
     * @brief Creates an action that will listen to the specified keys.
     * 
     * Overwrites any existing action with the same name.
     * 
     * @param action_name Name of the action to create
     * @param action_keys_list List of key codes to associate with this action
     */
    void create_action(const std::string &action_name,
        const std::list<int> &action_keys_list);

    /**
     * @brief Appends a key to an existing action.
     * 
     * Fails if the action does not exist.
     * Adding an existing key will have no effect.
     * 
     * @param action_name Name of the action to modify
     * @param action_key Key code to add to the action
     * @return True if the key was added; false if the action does not exist
     */
    bool add_key_to_action(const std::string &action_name, int action_key);
    
    /**
     * @brief Retrieves the keys associated with an action.
     * 
     * @param name Name of the action to query
     * @return The keys if the action exists; otherwise, std::nullopt
     */
    std::optional<std::unordered_set<int>> get_keys_from_action(
        const std::string &name) const;

    /**
     * @brief Deletes an existing action.
     * 
     * @param action_name Name of the action to delete
     * @return True if the action was removed; false if it did not exist
     */
    bool delete_action(const std::string &action_name);

    /**
     * @brief Checks whether an action with the specified name exists.
     * 
     * @param action_name Name of the action to check
     * @return True if the action exists; otherwise, false
     */
    bool has_action(const std::string &action_name) const;

    /**
     * @brief Checks whether any keys associated with the action are currently pressed.
     * 
     * @param action_name Name of the action to check
     * @return True if at least one key is currently pressed; otherwise, false
     */
    bool is_action_pressed(const std::string &action_name) const;
    
    /**
     * @brief Checks whether any keys associated with the action were just pressed in this frame.
     * 
     * Requires update() to be called once per frame.
     * 
     * @param action_name Name of the action to check
     * @return True if at least one key was just pressed; otherwise, false
     */
    bool is_action_just_pressed(const std::string& action_name) const;

    /**
     * @brief Checks whether any keys associated with the action were just released this frame.
     * 
     * Requires update() to be called once per frame.
     * 
     * @param action_name Name of the action to check
     * @return True if at least one key was just released; otherwise, false
     */
    bool is_action_just_released(const std::string &action_name) const;
};

#endif //SANDSTONE_INPUT_SYSTEM_H