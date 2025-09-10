#include "raylib.h"

#include <cstring>
#include <vector>
#include <string>

#include "core/simulation.h"
#include "systems/input_system.h"

constexpr int VIRTUAL_WIDTH  = 200;
constexpr int VIRTUAL_HEIGHT = 150;

constexpr int RES_SCALE = 5;
constexpr int WINDOW_WIDTH  = VIRTUAL_WIDTH * RES_SCALE;
constexpr int WINDOW_HEIGHT = VIRTUAL_HEIGHT * RES_SCALE;

struct Graphics {
    Texture2D canvas;
    Color *pixels;
};

class Application
{
public:
    Application()
    {
        _element_registry.initialize();
        _graphics = initialize_graphics(
            VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
            WINDOW_WIDTH, WINDOW_HEIGHT);

        for (const auto* type : _sim.get_all_element_types()) {
            if (type->get_id() != "EMPTY") {
                _type_ids.push_back(type->get_id());
            }
        }

        _input.create_action("place_element",
            { InputCode::mouse(MOUSE_LEFT_BUTTON) });
        _input.create_action(
            "cycle_element",
            { InputCode::key(KEY_SPACE) });
        _input.create_action(
            "prev_element",
            { InputCode::key(KEY_Q) });
        _input.create_action(
            "next_element",
            { InputCode::key(KEY_E) });
        _input.create_action(
            "erase_element",
            { InputCode::mouse(MOUSE_RIGHT_BUTTON) });
    }

    void run()
    {
        while (!WindowShouldClose()) {
            handle_input();
            _sim.step();
            _sim.fill_render_buffer(_graphics.pixels);
            draw_frame();
        }
        
        UnloadTexture(_graphics.canvas);
        MemFree(_graphics.pixels);
        CloseWindow();
    }

private:
    Graphics _graphics;
    
    ElementRegistry _element_registry { "WHATEVER FOR NOW" };
    Simulation _sim { VIRTUAL_WIDTH, VIRTUAL_HEIGHT, _element_registry };
    InputSystem _input;
    
    uint _brush_size = 1;
    std::vector<std::string> _type_ids;
    size_t _current_type_idx = 0;
    
    static Graphics initialize_graphics(const int virtual_width,
        const int virtual_height, const int window_width, const int window_height)
    {
        InitWindow(window_width, window_height, "Sandstone Simulation");
        SetTargetFPS(120);

        const Image init_image = GenImageColor(virtual_width, virtual_height, BLACK);
        const Texture2D canvas = LoadTextureFromImage(init_image);
        UnloadImage(init_image);
        SetTextureFilter(canvas, TEXTURE_FILTER_POINT);

        const auto pixels = static_cast<Color*>(MemAlloc(virtual_width * virtual_height * sizeof(Color)));
        memset(pixels, 0, virtual_width * virtual_height * sizeof(Color));

        return { canvas, pixels };
    }

    void draw_frame() const
    {
        UpdateTexture(_graphics.canvas, _graphics.pixels);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            _graphics.canvas,
            Rectangle{0, 0, static_cast<float>(VIRTUAL_WIDTH), static_cast<float>(VIRTUAL_HEIGHT)},
            Rectangle{0, 0, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)},
            Vector2{0, 0},
            0.0f,
            WHITE
        );
        draw_overlay();
        EndDrawing();
    }

    void draw_overlay() const
    {
        constexpr int margin = 8;
        constexpr int font_size = 5 * RES_SCALE;
        constexpr int pad = 1;

        Vector2I pos = { margin, margin };

        const std::string guide_label = "Q/E: Switch elements";
        DrawText(guide_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(guide_label.c_str(), pos.x, pos.y, font_size, WHITE);

        pos.y += font_size + pad;
        const std::string rmb_guide = "RMB: Erase";
        DrawText(rmb_guide.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(rmb_guide.c_str(), pos.x, pos.y, font_size, WHITE);

        pos.y += font_size + pad;
        const std::string scroll_guide = "Scroll: Brush size";
        DrawText(scroll_guide.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(scroll_guide.c_str(), pos.x, pos.y, font_size, WHITE);
        
        pos.y += font_size + pad*15;
        const std::string &current_type_id = _type_ids[_current_type_idx];
        const std::string type_label = "Element: " + current_type_id;
        DrawText(type_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(type_label.c_str(), pos.x, pos.y, font_size, GREEN);

        pos.y += font_size + pad;
        const std::string size_label = "Brush Size: " + std::to_string(_brush_size);;
        DrawText(size_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(size_label.c_str(), pos.x, pos.y, font_size, GREEN);
    }

    void draw_at_pos(const Vector2I &pos, const std::string& type_id, const int expand_brush = 0)
    { 
        const auto type = _sim.get_type_by_id(type_id);
        const bool erase = (type_id == "EMPTY");
        for (int x = pos.x - expand_brush; x < pos.x + expand_brush + 1; x++) {
            for (int y = pos.y - expand_brush; y < pos.y + expand_brush + 1; y++) {
                if (erase || _sim.is_pos_empty(x, y)) {
                    _sim.set_type_at(x, y, type, type->get_random_color_index());
                }
            }
        }
    }

    static size_t get_next_type_index(const size_t current, const size_t total)
    {
        return (current + 1) % total;
    }

    static size_t get_prev_type_index(const size_t current, const size_t total)
    {
        return (current + total - 1) % total;
    }

    void increase_brush_size()
    {
        if (_brush_size == INT_MAX)
            return;
        _brush_size++;
    }

    void decrease_brush_size()
    {
        if (_brush_size == 1)
            return;
        _brush_size--;   
    }

    void handle_input()
    {
        _input.update();
        
        auto [x_screen, y_screen] = _input.get_mouse_position();
        const Vector2 current_mouse_pos = {
            x_screen / RES_SCALE,
            y_screen / RES_SCALE
        };

        const std::string& current_type_id = _type_ids[_current_type_idx];

        if (_input.is_action_pressed("place_element")) {
            draw_at_pos(Vector2I(current_mouse_pos), current_type_id, _brush_size-1);
        }

        // RMB eraser paints EMPTY
        if (_input.is_action_pressed("erase_element")) {
            draw_at_pos(Vector2I(current_mouse_pos), "EMPTY", _brush_size-1);
        }
    
        if (_input.is_action_just_pressed("cycle_element")) {
            _current_type_idx = get_next_type_index(_current_type_idx, _type_ids.size());
        }

        // Q/E for prev/next element
        if (_input.is_action_just_pressed("prev_element")) {
            _current_type_idx = get_prev_type_index(_current_type_idx, _type_ids.size());
        }
        if (_input.is_action_just_pressed("next_element")) {
            _current_type_idx = get_next_type_index(_current_type_idx, _type_ids.size());
        }

        // Mouse wheel adjusts brush size
        const float wheel = _input.get_mouse_scroll_delta();
        constexpr float tolerance = 0.01f;
        if (wheel > tolerance) {
            increase_brush_size();
        } else if (wheel < -tolerance) {
            decrease_brush_size();
        }
    }
};

int main()
{
    Application _app;
    _app.run();
    
    return 0;
}
