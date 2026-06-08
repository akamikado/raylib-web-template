#include "raylib.h"

#include <emscripten/emscripten.h>

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(void);

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - window web");

    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    SetTargetFPS(60);

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void)
{
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Welcome to raylib web structure!", 220, 200, 20, SKYBLUE);

    EndDrawing();
}
