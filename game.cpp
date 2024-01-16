#include "raylib.h"
#include "raymath.h"
#include "character.h"
#include "prop.h"
#include "enemy.h"
#include <string>

int main()
{
    const int windowWidth = 384;
    const int windowHeight = 384;

    InitWindow(windowWidth, windowHeight, "Game 3");

    // Map
    Texture2D map = LoadTexture("tileset/map.png");
    Vector2 mapPos = {0.0, 0.0};
    const float mapScale{4.0f};

    // Draw knight
    Character knight{windowWidth, windowHeight};

    // Draw props
    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("naturetileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("naturetileset/Log.png")}};

    // Draw goblin
    Enemy goblin{
        Vector2{800.f, 300.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy slime{
        Vector2{700.f, 700.f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // Draw map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        // Draw health
        if (!knight.getAlive())
        {
            DrawText("Game Over", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        // Knight
        knight.tick(GetFrameTime());

        // Goblin
        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        // Map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // Colliding with props
        for (auto prop : props)
        {
            bool colliding = CheckCollisionRecs(knight.getCollisionRec(),
                                                prop.getCollisionRec(knight.getWorldPos()));
            if (colliding)
            {
                knight.undoMovement();
            }
        }

        // Check sword collision
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
}