#include "raylib.h"
#include <cstring>

constexpr int WIDTH  = 800;
constexpr int HEIGHT = 600;

int main() {
    InitWindow(WIDTH, HEIGHT, "Sandstone Simulation");
    SetTargetFPS(60);

    const Image initImage = GenImageColor(WIDTH, HEIGHT, BLACK);
    const Texture2D canvas = LoadTextureFromImage(initImage);
    UnloadImage(initImage);

    const auto pixels = static_cast<Color*>(MemAlloc(WIDTH * HEIGHT * sizeof(Color)));
    memset(pixels, 0, WIDTH * HEIGHT * sizeof(Color));

    while (!WindowShouldClose()) {
        UpdateTexture(canvas, pixels);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(canvas, 0, 0, BLACK);
        EndDrawing();
    }

    UnloadTexture(canvas);
    MemFree(pixels);
    CloseWindow();
    return 0;
}
