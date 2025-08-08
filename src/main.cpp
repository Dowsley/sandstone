#include "raylib.h"

#include <cstring>
#include <vector>
#include <string>

#include "core/simulation.h"

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
        _graphics = _initialize_graphics(
            VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
            WINDOW_WIDTH, WINDOW_HEIGHT);

        for (const auto* type : _sim.get_all_element_types()) {
            if (type->get_id() != "EMPTY") {
                _type_ids.push_back(type->get_id());
            }
        }
    }

    void run()
    {
        while (!WindowShouldClose()) {
            _handle_input();
            _sim.step();
            _sim.fill_render_buffer(_graphics.pixels);
            _draw_frame();
        }
        
        UnloadTexture(_graphics.canvas);
        MemFree(_graphics.pixels);
        CloseWindow();
    }

private:
    Graphics _graphics;
    
    ElementRegistry _element_registry { "WHATEVER FOR NOW" };
    Simulation _sim { VIRTUAL_WIDTH, VIRTUAL_HEIGHT, _element_registry };
    
    uint _brush_size = 1;
    std::vector<std::string> _type_ids;
    size_t _current_type_idx = 0;
    
    bool _cycle_key_prev = false;
    bool _increase_brush_key_prev = false;
    bool _decrease_brush_key_prev = false;

    static Graphics _initialize_graphics(const int virtual_width, const int virtual_height,
        const int window_width, const int window_height)
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

    void _draw_frame() const
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
        EndDrawing();
    }

    void _draw_at_pos(const Vector2I &pos, const std::string& type_id, const int expand_brush = 0)
    { 
        // TODO: Encapsulate this under a `within_bounds` func
        for (int x = pos.x - expand_brush; x < pos.x + expand_brush + 1; x++) {
            for (int y = pos.y - expand_brush; y < pos.y + expand_brush + 1; y++) {
                if (x >= 0 && x < VIRTUAL_WIDTH && y >= 0 && y < VIRTUAL_HEIGHT) {
                    if (_sim.is_pos_empty(x, y)) {
                        const auto type = _sim.get_type_by_id(type_id);
                        _sim.set_type_at(x, y, type, type->get_random_color_index());
                    }
                }
            }
        }
    }

    static size_t _get_next_type_index(const size_t current, const size_t total)
    {
        return (current + 1) % total;
    }

    void _handle_input()
    {
        auto [x, y] = GetMousePosition();
        const Vector2 current_mouse_pos = {
            x / RES_SCALE,
            y / RES_SCALE
        };

        const std::string& current_type_id = _type_ids[_current_type_idx];

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            _draw_at_pos(Vector2I(current_mouse_pos), current_type_id, _brush_size-1);
        }
    
        const bool cycle_key = IsKeyDown(KEY_SPACE);
        if (cycle_key && !_cycle_key_prev) {
            _current_type_idx = _get_next_type_index(_current_type_idx, _type_ids.size());
        }
        _cycle_key_prev = cycle_key;

        const bool increase_brush_key = IsKeyDown(KEY_TAB);
        if (increase_brush_key && !_increase_brush_key_prev) {
            _brush_size++;
        }
        _increase_brush_key_prev = increase_brush_key;

        const bool decrease_brush_key = IsKeyDown(KEY_LEFT_SHIFT);
        if (decrease_brush_key && !_decrease_brush_key_prev) {
            if (_brush_size > 1)
                _brush_size--;
        }
        _decrease_brush_key_prev = decrease_brush_key;
    }
};

int main()
{
    Application _app;
    _app.run();
    
    return 0;
}
