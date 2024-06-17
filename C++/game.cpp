#include "raylib.h"

struct AnimData
{
    Rectangle Rec;
    Vector2 Pos;
    int Frame;
    float Updatetime;
    float RunningTime;
};
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.RunningTime += deltaTime;
    if (data.RunningTime >= data.Updatetime)
    {
        data.RunningTime = 0.0;
        data.Rec.x = data.Frame * data.Rec.width;
        data.Frame++;
        if (data.Frame > maxFrame)
        {
            data.Frame = 0;
        }
    }
    return data;
}

bool isOnGround(AnimData data, int windowDimensions)
{
    return data.Pos.y >= windowDimensions - data.Rec.height;
}

int main()
{

    int windowDimensions[2];
    windowDimensions[0] = 800;
    windowDimensions[1] = 600;
    InitWindow(windowDimensions[0], windowDimensions[1], "Raylib Game");

    const int gravity{1'000};
    const int jumpVel{-600};
    bool isInAir{};
    int velocity{0}; // pixels/Frame
    const int SizeOfNebulae{6};

    int nebInterval = 300;
    // scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.Rec.width = scarfy.width / 6;
    scarfyData.Rec.height = scarfy.height;
    scarfyData.Rec.x = 0;
    scarfyData.Rec.y = 0;
    scarfyData.Pos.x = windowDimensions[0] / 2 - scarfyData.Rec.width / 2;
    scarfyData.Pos.y = windowDimensions[1] - scarfyData.Rec.height;
    scarfyData.Frame = 0;
    scarfyData.Updatetime = 1.0 / 12.0;
    scarfyData.RunningTime = 0.0;
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    int nebvel{-200};

    AnimData nebulae[SizeOfNebulae]{};

    for (int i = 0; i < SizeOfNebulae; i++)
    {

        nebulae[i].Rec.x = 0;
        nebulae[i].Rec.y = 0;
        nebulae[i].Rec.width = nebula.width / 8;
        nebulae[i].Rec.height = nebula.height / 8;
        nebulae[i].Pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].RunningTime = 0.0;
        nebulae[i].Updatetime = 0;
        nebulae[i].Pos.x = windowDimensions[0] + i * nebInterval;
    }
    float finishLine{nebulae[SizeOfNebulae - 1].Pos.x};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float dt{GetFrameTime()};

        BeginDrawing();

        ClearBackground(RAYWHITE);
        bgX -= 20 * dt;
        fgX -= 40 * dt;
        mgX -= 80 * dt;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        Vector2 bg1Pos = {bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 3.5, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 3.5, WHITE);

        Vector2 fg1Pos = {fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 3.5, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.5, WHITE);

        Vector2 mg1Pos = {mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 3.5, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 3.5, WHITE);
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dt, 5);
        }
        // ground collision check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // apply gravity
            // Rectangle is in the air
            velocity += gravity * dt;
            isInAir = true;
        }
        // check for jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
        // update Position
        for (int i = 0; i < SizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dt, 8);
            nebulae[i].Pos.x += nebvel * dt;
        }
        bool collision{};
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.Pos.x + pad,
                nebula.Pos.y + pad,
                nebula.Rec.width - 2 * pad,
                nebula.Rec.height - 2 * pad};
            Rectangle scarfyRec{
                scarfyData.Pos.x,
                scarfyData.Pos.y,
                scarfyData.Rec.width,
                scarfyData.Rec.height};
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }
        if (collision)
        {  
            DrawText("Game Over", windowDimensions[0] / 2 - MeasureText("Game Over", 50) / 2, windowDimensions[1] / 2, 50, RED);
        }
        else if (scarfyData.Pos.x >= finishLine)
        {
            DrawText("You Win", windowDimensions[0] / 2 - MeasureText("You Win", 50) / 2, windowDimensions[1] / 2, 50, GREEN);
        }

        else
        {DrawFPS(10, 10);
            for (int i = 0; i < SizeOfNebulae; i++)
            {

                DrawTextureRec(nebula, nebulae[i].Rec, nebulae[i].Pos, WHITE);
            }
            DrawTextureRec(scarfy, scarfyData.Rec, scarfyData.Pos, WHITE);
        }
        finishLine += nebvel * dt;
        scarfyData.Pos.y += velocity * dt;
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}