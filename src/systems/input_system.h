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
#include <functional>
#include <raylib.h>

/**
 * @brief Represents either a keyboard key or mouse button input.
 * 
 * Wraps integer codes to distinguish between keyboard keys and mouse buttons,
 * since both use integer representations in Raylib.
 */
struct InputCode {
    enum Type { KEY, MOUSE_BUTTON };
    
    Type type;
    int code;
    
    InputCode(const Type t, const int c) : type(t), code(c) {}
    
    // Convenience constructors
    static InputCode key(const int keycode) { return InputCode(KEY, keycode); }
    static InputCode mouse(const int button) { return InputCode(MOUSE_BUTTON, button); }
    
    // For use in unordered containers
    bool operator==(const InputCode& other) const {
        return type == other.type && code == other.code;
    }
};

// Hash function for InputCode to use in unordered_set/map.
struct InputCodeHash {
    std::size_t operator()(const InputCode& input) const {
        return std::hash<int>()(input.type) ^ (std::hash<int>()(input.code) << 1);
    }
};

/**
 * @brief Handles inputs for RayLib in the same style as Godot's Input class, based on named actions.
 * 
 * @see https://www.raylib.com/cheatsheet/cheatsheet.html
 */
class InputSystem
{
private:
    // TODO: Wrap these unordered sets into a "State" type
    std::unordered_map<std::string, std::unordered_set<InputCode, InputCodeHash>> _actions;
    std::unordered_set<InputCode, InputCodeHash> _previous_input_state;
    std::unordered_set<InputCode, InputCodeHash> _current_input_state;

    // Cache of all inputs referenced by any action
    std::unordered_set<InputCode, InputCodeHash> _watched_inputs;
    
    // Mouse state tracking
    Vector2 _current_mouse_position = {};
    Vector2 _previous_mouse_position = {};
    float _current_mouse_wheel = 0;
    float _previous_mouse_wheel = 0;
    
    void rebuild_watched_inputs();
    bool is_input_down(const InputCode& input) const;
    bool is_input_up(const InputCode& input) const;
    bool was_input_down(const InputCode& input) const;
    bool was_input_up(const InputCode& input) const;

public:
    /**
     * @brief Updates the internal keyboard and mouse states.
     * 
     * Must be called once per frame to enable edge detection (just pressed/released).
     */
    void update();

    /**
     * @brief Creates an action that will listen to the specified inputs.
     * 
     * Overwrites any existing action with the same name.
     * 
     * @param action_name Name of the action to create
     * @param action_inputs_list List of InputCodes (keys/mouse buttons) to associate with this action
     */
    void create_action(const std::string &action_name,
        const std::list<InputCode> &action_inputs_list);

    /**
     * @brief Appends an input to an existing action.
     * 
     * Fails if the action does not exist.
     * Adding an existing input will have no effect.
     * 
     * @param action_name Name of the action to modify
     * @param action_input InputCode (key/mouse button) to add to the action
     * @return True if the input was added; false if the action does not exist
     */
    bool add_input_to_action(const std::string &action_name, const InputCode &action_input);
    
    /**
     * @brief Retrieves the inputs associated with an action.
     * 
     * @param name Name of the action to query
     * @return The inputs if the action exists; otherwise, std::nullopt
     */
    std::optional<std::unordered_set<InputCode, InputCodeHash>> get_inputs_from_action(
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


    /**
     * @brief Gets the current mouse position in screen coordinates.
     * 
     * Requires update() to be called once per frame.
     * 
     * @return A Vector2 representing the mouse's X and Y coordinates
     */
    Vector2 get_mouse_position() const;

    /**
     * @brief Gets the change in mouse position since the last frame.
     * 
     * Requires update() to be called once per frame.
     * 
     * @return A Vector2 representing the delta in mouse X and Y coordinates
     */
    Vector2 get_mouse_delta() const;

    /**
     * @brief Gets the change in mouse scroll-wheel value since the last frame.
     * 
     * Requires update() to be called once per frame.
     * 
     * @return The difference in scroll-wheel value
     */
    float get_mouse_scroll_delta() const;
};

#endif //SANDSTONE_INPUT_SYSTEM_H