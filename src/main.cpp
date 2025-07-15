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

int main()
{
    auto sim = Simulation(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    const auto [canvas, pixels] = initialize_graphics(
        VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
        WINDOW_WIDTH, WINDOW_HEIGHT);
    while (!WindowShouldClose()) {
        sim.step();
        sim.fill_render_buffer(pixels);
        draw_frame(canvas, pixels);
    }
    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
