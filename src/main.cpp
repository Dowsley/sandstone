#include "raylib.h"

#include <cstring>

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

CellType get_next_type(CellType type)
{
    constexpr int FIRST = static_cast<int>(CellType::SAND);
    constexpr int LAST = static_cast<int>(CellType::WATER);
    int next = static_cast<int>(type) + 1;
    if (next > LAST) next = FIRST;
    return static_cast<CellType>(next);
}

/*
 * Helper function to draw a line of particles using Bresenham's algorithm.
* This ensures that fast mouse movements create a solid line.
*/
void draw_particle_line(
    Simulation &sim, const Vector2 start, const Vector2 end, const CellType type)
{
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    const int x1 = static_cast<int>(end.x);
    const int y1 = static_cast<int>(end.y);

    const int dx = abs(x1 - x0);
    const int sx = x0 < x1 ? 1 : -1;
    const int dy = -abs(y1 - y0);
    const int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        // Place a particle at the current point on the line
        if (x0 >= 0 && x0 < VIRTUAL_WIDTH && y0 >= 0 && y0 < VIRTUAL_HEIGHT) {
            if (sim.get_type_at(x0, y0) == CellType::EMPTY) {
                sim.set_type_at(x0, y0, type);
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


void handle_input(Simulation &sim, CellType &current_type, bool &cycle_key_prev, Vector2 &prev_mouse_pos)
{
    auto [x, y] = GetMousePosition();
    const Vector2 current_mouse_pos = {
        x / RES_SCALE,
        y / RES_SCALE
    };

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // If prev_mouse_pos has been reset, it means this is the first frame of a new click.
        // We draw a line from the current position to itself (a single point).
        // Otherwise, we draw from the last frame's position to the current one.
        const Vector2 start_pos = (prev_mouse_pos.x == -1.0f) ? current_mouse_pos : prev_mouse_pos;
        draw_particle_line(sim, start_pos, current_mouse_pos, current_type);

        // Update the previous position for the next frame.
        prev_mouse_pos = current_mouse_pos;
    }
    else {
        // If the mouse button is not pressed, reset the previous position.
        // This ensures the next click doesn't connect to the last release point.
        prev_mouse_pos = {-1.0f, -1.0f};
    }
    
    const bool cycle_key = IsKeyDown(KEY_SPACE);
    if (cycle_key && !cycle_key_prev) {
        current_type = get_next_type(current_type);
    }
    cycle_key_prev = cycle_key;
}

int main()
{
    auto sim = Simulation(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    const auto [canvas, pixels] = initialize_graphics(
        VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
        WINDOW_WIDTH, WINDOW_HEIGHT);
    auto current_type = CellType::SAND;
    bool cycle_key_prev = false;
    
    // Stores the mouse position from the previous frame to draw lines.
    // Initialized to an invalid position.
    Vector2 prev_mouse_pos = {-1.0f, -1.0f};
    while (!WindowShouldClose()) {
        handle_input(sim, current_type, cycle_key_prev, prev_mouse_pos);
        sim.step();
        sim.fill_render_buffer(pixels);
        draw_frame(canvas, pixels);
    }
    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
