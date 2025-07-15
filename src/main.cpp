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

Graphics InitializeGraphics(const int virtualWidth, const int virtualHeight,
    const int windowWidth, const int windowHeight)
{
    InitWindow(windowWidth, windowHeight, "Sandstone Simulation");
    SetTargetFPS(60);

    const Image initImage = GenImageColor(virtualWidth, virtualHeight, BLACK);
    const Texture2D canvas = LoadTextureFromImage(initImage);
    UnloadImage(initImage);
    SetTextureFilter(canvas, TEXTURE_FILTER_POINT);

    const auto pixels = static_cast<Color*>(MemAlloc(virtualWidth * virtualHeight * sizeof(Color)));
    memset(pixels, 0, virtualWidth * virtualHeight * sizeof(Color));

    return { canvas, pixels };
}

int main()
{
    auto sim = Simulation(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    const auto [canvas, pixels] = InitializeGraphics(
        VIRTUAL_WIDTH, VIRTUAL_HEIGHT,
        WINDOW_WIDTH, WINDOW_HEIGHT);
    while (!WindowShouldClose()) {
        sim.Step();
        sim.FillRenderBuffer(pixels);

        UpdateTexture(canvas, pixels);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            canvas,
            Rectangle {0, 0,
                static_cast<float>(VIRTUAL_WIDTH), static_cast<float>(VIRTUAL_HEIGHT) },
            Rectangle{0, 0,
                static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)},
            Vector2{0, 0},
            0.0f,
            WHITE
        );
        EndDrawing();
    }

    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
