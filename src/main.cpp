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

Graphics initialize_graphics(const int virtual_width, const int virtual_height,
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

void draw_frame(const Texture2D &canvas, const Color *pixels)
{
    UpdateTexture(canvas, pixels);
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        canvas,
        Rectangle{0, 0, static_cast<float>(VIRTUAL_WIDTH), static_cast<float>(VIRTUAL_HEIGHT)},
        Rectangle{0, 0, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)},
        Vector2{0, 0},
        0.0f,
        WHITE
    );
    EndDrawing();
}

void draw_at_pos(
    Simulation &sim, const Vector2I &pos, const std::string& type_id, const int expand_brush = 0)
{
    // TODO: Implement checking here
    for (int x = pos.x - expand_brush; x < pos.x + expand_brush + 1; x++) {
        for (int y = pos.y - expand_brush; y < pos.y + expand_brush + 1; y++) {
            if (x >= 0 && x < VIRTUAL_WIDTH && y >= 0 && y < VIRTUAL_HEIGHT) {
                if (sim.is_pos_empty(x, y)) {
                    const auto type = sim.get_type_by_id(type_id);
                    sim.set_type_at(x, y, type, type->get_random_color_index());
                }
            }
        }
    }
}

size_t get_next_type_index(const size_t current, const size_t total)
{
    return (current + 1) % total;
}

void handle_input(Simulation &sim, const std::vector<std::string> &type_ids,
    size_t &current_type_idx, bool &cycle_key_prev, bool &increase_brush_key_prev,
    bool &decrease_brush_key_prev, uint &brush_size)
{
    auto [x, y] = GetMousePosition();
    const Vector2 current_mouse_pos = {
        x / RES_SCALE,
        y / RES_SCALE
    };

    const std::string& current_type_id = type_ids[current_type_idx];

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        draw_at_pos(sim, Vector2I(current_mouse_pos), current_type_id, brush_size-1);
    }
    
    const bool cycle_key = IsKeyDown(KEY_SPACE);
    if (cycle_key && !cycle_key_prev) {
        current_type_idx = get_next_type_index(current_type_idx, type_ids.size());
    }
    cycle_key_prev = cycle_key;

    const bool increase_brush_key = IsKeyDown(KEY_TAB);
    if (increase_brush_key && !increase_brush_key_prev) {
        brush_size++;
    }
    increase_brush_key_prev = increase_brush_key;

    const bool decrease_brush_key = IsKeyDown(KEY_LEFT_SHIFT);
    if (decrease_brush_key && !decrease_brush_key_prev) {
        if (brush_size > 1) brush_size--;
    }
    decrease_brush_key_prev = decrease_brush_key;
}

int main()
{
    uint brush_size = 1;
    
    ElementRegistry element_registry { "WHATEVER FOR NOW" };
    element_registry.initialize();
    
    Simulation sim(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, element_registry);
    const auto [canvas, pixels] = initialize_graphics(
        VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
        WINDOW_WIDTH, WINDOW_HEIGHT);

    std::vector<std::string> type_ids;
    for (const auto* type : sim.get_all_element_types()) {
        if (type->get_id() != "EMPTY") {
            type_ids.push_back(type->get_id());
        }
    }
    
    size_t current_type_idx = 0;
    bool cycle_key_prev = false;
    bool increase_brush_key_prev = false;
    bool decrease_brush_key_prev = false;
    while (!WindowShouldClose()) {
        handle_input(sim, type_ids, current_type_idx, cycle_key_prev,
            increase_brush_key_prev, decrease_brush_key_prev, brush_size);
        sim.step();
        sim.fill_render_buffer(pixels);
        draw_frame(canvas, pixels);
    }
    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
