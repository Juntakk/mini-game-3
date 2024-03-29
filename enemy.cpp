#include "raylib.h"
#include "raymath.h"
#include "enemy.h"
#include "character.h"

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = texture.width / maxFrames;
    height = texture.height;
    speed = 3.5f;
};

void Enemy::tick(float deltaTime)
{
    if (!getAlive())
        return;

    // get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if (Vector2Length(velocity) < radius)
        velocity = {};

    BaseCharacter::tick(deltaTime);

    // Do damage to player
    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        target->takeDamage(getDamagePerSec() * deltaTime);
    }
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}
