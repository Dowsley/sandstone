#include "raylib.h"

#include <cstring>
#include <vector>
#include <string>

#include "core/simulation.h"
#include "systems/input_system.h"
#include "utils/random_utils.h"

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
            "prev_element",
            { InputCode::key(KEY_Q) });
        _input.create_action(
            "next_element",
            { InputCode::key(KEY_E) });
        _input.create_action(
            "erase_element",
            { InputCode::mouse(MOUSE_RIGHT_BUTTON) });
        _input.create_action(
            "cycle_brush_shape",
            { InputCode::key(KEY_TAB) });
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
    
    uint _brush_size = 5;
    enum class BrushShape { square = 0, round = 1, spray = 2 };
    BrushShape _brush_shape = BrushShape::round;
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
        draw_cursor_outline();
        draw_overlay();
        EndDrawing();
    }

    static const std::string& brush_shape_name(const BrushShape s)
    {
        static const std::string BRUSH_NAME_SQUARE = "Square";
        static const std::string BRUSH_NAME_ROUND  = "Round";
        static const std::string BRUSH_NAME_SPRAY  = "Spray";
        switch (s) {
            case BrushShape::square: return BRUSH_NAME_SQUARE;
            case BrushShape::round:  return BRUSH_NAME_ROUND;
            case BrushShape::spray:  return BRUSH_NAME_SPRAY;
        }
        return BRUSH_NAME_SQUARE;
    }

    void draw_overlay() const
    {
        constexpr int margin = 8;
        constexpr int font_size = 5 * RES_SCALE;
        constexpr int pad = 1;

        // Top-right FPS counter
        const int fps = GetFPS();
        const std::string fps_label = "FPS: " + std::to_string(fps);
        const int fps_width = MeasureText(fps_label.c_str(), font_size);
        const int fps_x = WINDOW_WIDTH - margin - fps_width;
        constexpr int fps_y = margin;
        DrawText(fps_label.c_str(), fps_x + 1, fps_y + 1, font_size, BLACK);
        DrawText(fps_label.c_str(), fps_x, fps_y, font_size, WHITE);

        Vector2I pos = { margin, margin };

        const std::string guide_label = "Q/E: Switch elements";
        DrawText(guide_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(guide_label.c_str(), pos.x, pos.y, font_size, WHITE);

        pos.y += font_size + pad;
        const std::string rmb_guide_label = "RMB: Erase";
        DrawText(rmb_guide_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(rmb_guide_label.c_str(), pos.x, pos.y, font_size, WHITE);

        pos.y += font_size + pad;
        const std::string scroll_guide_label = "Scroll: Brush size";
        DrawText(scroll_guide_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(scroll_guide_label.c_str(), pos.x, pos.y, font_size, WHITE);

        pos.y += font_size + pad;
        const std::string brush_mode_guide_label = "Brush Mode: Tab";
        DrawText(brush_mode_guide_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(brush_mode_guide_label.c_str(), pos.x, pos.y, font_size, WHITE);
        
        pos.y += font_size + pad+10;
        const std::string &current_type_id = _type_ids[_current_type_idx];
        const std::string type_label = "Element: " + current_type_id;
        DrawText(type_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(type_label.c_str(), pos.x, pos.y, font_size, GREEN);

        pos.y += font_size + pad;
        const std::string size_label = "Brush Size: " + std::to_string(_brush_size);;
        DrawText(size_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(size_label.c_str(), pos.x, pos.y, font_size, GREEN);

        pos.y += font_size + pad;
        const std::string shape_label = std::string("Brush Shape: ") + brush_shape_name(_brush_shape);
        DrawText(shape_label.c_str(), pos.x + 1, pos.y + 1, font_size, BLACK);
        DrawText(shape_label.c_str(), pos.x, pos.y, font_size, GREEN);
    }

    void draw_cursor_outline() const
    {
        const auto [x, y] = GetMousePosition();
        const int mx = static_cast<int>(x) / RES_SCALE;
        const int my = static_cast<int>(y) / RES_SCALE;
        const int expand = static_cast<int>(_brush_size) - 1;

        constexpr auto outline_color = GRAY;

        switch (_brush_shape) {
            case BrushShape::square: {
                const int left = (mx - expand) * RES_SCALE;
                const int top = (my - expand) * RES_SCALE;
                const int w = (expand * 2 + 1) * RES_SCALE;
                const int h = (expand * 2 + 1) * RES_SCALE;
                DrawRectangleLines(left, top, w, h, outline_color);
                break;
            }
            case BrushShape::round:
            case BrushShape::spray: {
                const int cx = mx * RES_SCALE + RES_SCALE / 2;
                const int cy = my * RES_SCALE + RES_SCALE / 2;
                const float r = (static_cast<float>(expand) + 0.5f) * RES_SCALE;
                DrawCircleLines(cx, cy, r, outline_color);
                break;
            }
        }
    }

    void draw_square(const Vector2I &pos, const std::string &type_id, const int half_extent)
    {
        const auto type = _sim.get_type_by_id(type_id);
        const bool erase = (type_id == "EMPTY");
        for (int x = pos.x - half_extent; x <= pos.x + half_extent; ++x) {
            for (int y = pos.y - half_extent; y <= pos.y + half_extent; ++y) {
                if (erase || _sim.is_pos_empty(x, y)) {
                    _sim.set_type_at(x, y, type, type->get_random_color_index());
                }
            }
        }
    }

    void draw_round(const Vector2I &pos, const std::string &type_id, const int radius)
    {
        const auto type = _sim.get_type_by_id(type_id);
        const bool erase = (type_id == "EMPTY");
        const int r2 = radius * radius;
        for (int dx = -radius; dx <= radius; ++dx) {
            for (int dy = -radius; dy <= radius; ++dy) {
                if (dx*dx + dy*dy <= r2) {
                    const int x = pos.x + dx;
                    const int y = pos.y + dy;
                    if (erase || _sim.is_pos_empty(x, y)) {
                        _sim.set_type_at(x, y, type, type->get_random_color_index());
                    }
                }
            }
        }
    }

    void draw_spray(const Vector2I &pos, const std::string &type_id, const int radius)
    {
        const auto type = _sim.get_type_by_id(type_id);
        const bool erase = (type_id == "EMPTY");
        constexpr int coverage = 15; // percent
        const int r2 = radius * radius;
        for (int dx = -radius; dx <= radius; ++dx) {
            for (int dy = -radius; dy <= radius; ++dy) {
                if (dx*dx + dy*dy <= r2) {
                    if (RandomUtils::uniform_int(0, 99) < coverage) {
                        const int x = pos.x + dx;
                        const int y = pos.y + dy;
                        if (erase || _sim.is_pos_empty(x, y)) {
                            _sim.set_type_at(x, y, type, type->get_random_color_index());
                        }
                    }
                }
            }
        }
    }

    void draw_at_pos(const Vector2I &pos, const std::string& type_id, const int expand_brush = 0)
    {
        switch (_brush_shape) {
            case BrushShape::square:
                draw_square(pos, type_id, expand_brush);
                break;
            case BrushShape::round:
                draw_round(pos, type_id, expand_brush);
                break;
            case BrushShape::spray:
                draw_spray(pos, type_id, expand_brush);
                break;
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

        if (_input.is_action_pressed("erase_element")) {
            draw_at_pos(Vector2I(current_mouse_pos), "EMPTY", _brush_size-1);
        }

        if (_input.is_action_just_pressed("prev_element")) {
            _current_type_idx = get_prev_type_index(_current_type_idx, _type_ids.size());
        }
        
        if (_input.is_action_just_pressed("next_element")) {
            _current_type_idx = get_next_type_index(_current_type_idx, _type_ids.size());
        }

        if (_input.is_action_just_pressed("cycle_brush_shape")) {
            const int next = (static_cast<int>(_brush_shape) + 1) % 3;
            _brush_shape = static_cast<BrushShape>(next);
        }

        // Mouse wheel adjusts brush size
        const float wheel = _input.get_mouse_scroll_delta();
        constexpr float tolerance = 0.01f;
        if (wheel > tolerance) {
            decrease_brush_size();
        } else if (wheel < -tolerance) {
            increase_brush_size();
        }
    }
};

int main()
{
    Application _app;
    _app.run();
    
    return 0;
}
