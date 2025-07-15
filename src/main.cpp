#include "raylib.h"

#include <cstdio>
#include <cstring>

constexpr int VIRTUAL_WIDTH  = 200;
constexpr int VIRTUAL_HEIGHT = 150;

constexpr int RES_SCALE = 5;
constexpr int WINDOW_WIDTH  = VIRTUAL_WIDTH * RES_SCALE;
constexpr int WINDOW_HEIGHT = VIRTUAL_HEIGHT * RES_SCALE;

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sandstone Simulation");
    SetTargetFPS(60);

    const Image initImage = GenImageColor(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, BLACK);
    const Texture2D canvas = LoadTextureFromImage(initImage);
    UnloadImage(initImage);
    SetTextureFilter(canvas, TEXTURE_FILTER_POINT);

    const auto pixels = static_cast<Color*>(
        MemAlloc(VIRTUAL_WIDTH * VIRTUAL_HEIGHT * sizeof(Color)));

    memset(pixels, 0, VIRTUAL_WIDTH * VIRTUAL_HEIGHT * sizeof(Color));
    for (int y = 0; y < VIRTUAL_HEIGHT; ++y) {
        for (int x = 0; x < VIRTUAL_WIDTH; ++x) {
            const int idx = y * VIRTUAL_WIDTH + x;
            pixels[idx].r = static_cast<unsigned char>(GetRandomValue(0, 255));
            pixels[idx].g = static_cast<unsigned char>(GetRandomValue(0, 255));
            pixels[idx].b = static_cast<unsigned char>(GetRandomValue(0, 255));
            pixels[idx].a = 255;
        }
    }

    while (!WindowShouldClose()) {
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
