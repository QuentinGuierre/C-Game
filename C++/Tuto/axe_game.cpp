#include "raylib.h"
#include <iostream>
#include <string>

int main()
{
    const int width = 500;
    const int height = 300;

    int circle_x{10};
    int circle_y{10};
    int collision{10};

    SetTargetFPS(60);
    InitWindow(width, height, "first game");

    while (WindowShouldClose() == false || IsKeyPressed('Escape'))
    {

        BeginDrawing();
        ClearBackground(BLUE);
        //  DrawCircle(GetMouseX(), GetMouseY(), 10, RED);
        DrawCircle(circle_x, circle_y, collision, RED);
        if (IsKeyDown(KEY_D) && circle_x + collision < width)
        {

            circle_x = circle_x + 1;
        }
        if (IsKeyDown(KEY_Q) && circle_y + collision < height)
        {
            circle_y = circle_y + 1;
        }
      
           // DrawText("Game Over", 10, 10, 16, GREEN);

        EndDrawing();
    }
}
