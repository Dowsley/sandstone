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
    SetTargetFPS(60);

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

// Helper function to draw a line of particles using Bresenham's algorithm.
// This ensures that fast mouse movements create a solid line.
void draw_particle_line(
    Simulation &sim, const Point start, const Point end, const std::string& type_id)
{
    int x0 = start.x;
    int y0 = start.y;
    const int x1 = end.x;
    const int y1 = end.y;

    const int dx = abs(x1 - x0);
    const int sx = x0 < x1 ? 1 : -1;
    const int dy = -abs(y1 - y0);
    const int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (x0 >= 0 && x0 < VIRTUAL_WIDTH && y0 >= 0 && y0 < VIRTUAL_HEIGHT) {
            if (const auto cell_type = sim.get_type_at(x0, y0); cell_type && cell_type->get_id() == "EMPTY") {
                const auto type = sim.get_type_by_id(type_id);
                sim.set_type_at(x0, y0, type, type->get_random_color_index());
            }
        }

        if (x0 == x1 && y0 == y1) break;
        const int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

size_t get_next_type_index(const size_t current, const size_t total)
{
    return (current + 1) % total;
}

void handle_input(Simulation &sim, const std::vector<std::string> &type_ids,
    size_t &current_type_idx, bool &cycle_key_prev, Vector2 &prev_mouse_pos)
{
    auto [x, y] = GetMousePosition();
    const Vector2 current_mouse_pos = {
        x / RES_SCALE,
        y / RES_SCALE
    };

    const std::string& current_type_id = type_ids[current_type_idx];

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        const Point start_pos = prev_mouse_pos.x == -1.0f
            ? Point(current_mouse_pos)
            : Point(prev_mouse_pos);
        draw_particle_line(sim, start_pos, Point(current_mouse_pos), current_type_id);
        prev_mouse_pos = current_mouse_pos;
    }
    else {
        prev_mouse_pos = {-1.0f, -1.0f};
    }
    
    const bool cycle_key = IsKeyDown(KEY_SPACE);
    if (cycle_key && !cycle_key_prev) {
        current_type_idx = get_next_type_index(current_type_idx, type_ids.size());
    }
    cycle_key_prev = cycle_key;
}

int main()
{
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
    Vector2 prev_mouse_pos = {-1.0f, -1.0f};
    while (!WindowShouldClose()) {
        printf("%s\n", type_ids[current_type_idx].c_str());
        handle_input(sim, type_ids, current_type_idx, cycle_key_prev, prev_mouse_pos);
        sim.step();
        sim.fill_render_buffer(pixels);
        draw_frame(canvas, pixels);
    }
    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
