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
    constexpr int FIRST = static_cast<int>(CellType::EMPTY);
    constexpr int LAST = static_cast<int>(CellType::WATER);
    int next = static_cast<int>(type) + 1;
    if (next > LAST) next = FIRST;
    return static_cast<CellType>(next);
}

void handle_input(Simulation &sim, CellType &current_type, bool &cycle_key_prev)
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        auto [x, y] = GetMousePosition();
        x = static_cast<int>(x / RES_SCALE);
        y = static_cast<int>(y / RES_SCALE);
        if (x >= 0 && x < VIRTUAL_WIDTH && y >= 0 && y < VIRTUAL_HEIGHT) {
            sim.set_type_at(x, y, current_type);
        }
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
    while (!WindowShouldClose()) {
        handle_input(sim, current_type, cycle_key_prev);
        sim.step();
        sim.fill_render_buffer(pixels);
        draw_frame(canvas, pixels);
    }
    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
