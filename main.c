#include "raylib.h"

int main(void)
{
    // Initialize the window
    InitWindow(800, 600, "Hello Anonymous!");

    // Set the target frames per second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello Anonymous!", 200, 200, 40, ORANGE);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
