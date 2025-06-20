#include "circle_shape.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS     "playstation"

const int PLAYER_RADIUS = 20;
const int PLAYER_TURN_SPEED = 5;
const int PLAYER_SPEED = 10;
const int PLAYER_SHOOT_SPEED = 25;
const float PLAYER_SHOOT_COOLDOWN = 0.35f; // seconds
const float SHOT_COOLDOWN = 1.0f;

const int SHOT_RADIUS = 5;

void createPlayer(Player *p){
	CircleShape shape = {
		{500, 500}, PLAYER_RADIUS, WHITE, {0, 0}
	};

	p->shape = shape;
	p->rotation = 0.0f;
    p->shots = createShotsArray(1);
    p->shotCount = 0;
    p->timer = 0.0f;
}

Vector2* getTriangle(Player p){
    Vector2* vertices = (Vector2*)malloc(3 * sizeof(Vector2));
    float angle = p.rotation;
    float radius = p.shape.radius;

    // Tip of the triangle (forward)
    vertices[0] = Vector2Add(p.shape.position, Vector2Rotate((Vector2){0, -radius}, angle));
    // Left base
    vertices[1] = Vector2Add(p.shape.position, Vector2Rotate((Vector2){-radius * 0.6f, radius * 0.8f}, angle));
    // Right base
    vertices[2] = Vector2Add(p.shape.position, Vector2Rotate((Vector2){radius * 0.6f, radius * 0.8f}, angle));

    return vertices;
}

void drawPlayer(Player player){
    Vector2 *vertices = getTriangle(player);
    DrawTriangle(vertices[0], vertices[1], vertices[2], WHITE);
    free(vertices);
}

void rotatePlayer(Player *player, float dt){
    player->rotation += PLAYER_TURN_SPEED * dt;
}

void movePlayer(Player *player, float dt){
    float angle = player->rotation;
    float radius = player->shape.radius;
    Vector2 forward = Vector2Rotate((Vector2){0, -radius}, angle);
    if(dt > 0){
        player->shape.position = Vector2Add(player->shape.position, Vector2Scale(forward, PLAYER_SPEED * dt));
    }
    else{
        player->shape.position = Vector2Add(player->shape.position, Vector2Scale(forward, PLAYER_SPEED/1.5 * dt));
    }
}


void shoot(Player *player, int dt){
    float angle = player->rotation;
    float radius = player->shape.radius;
    Vector2 forward = Vector2Rotate((Vector2){0, -radius}, angle);
    CircleShape shape = {
		{player->shape.position.x, player->shape.position.y}, SHOT_RADIUS, WHITE, Vector2Scale(forward, PLAYER_SHOOT_SPEED)
	};
    Shot shot = {player->shotCount, shape, SHOT_COOLDOWN};
    insertShot(player->shots, shot);
    player->timer = PLAYER_SHOOT_COOLDOWN;
    // DrawCircle(shot.shape.position.x, shot.shape.position.y, shot.shape.radius, WHITE);
    player->shotCount++;
}

int getGamepadEvents(int gamepad, Player *player){
    // Set axis deadzones
    const float leftStickDeadzoneX = 0.3f;
    const float leftStickDeadzoneY = 0.3f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;
    const float leftTriggerDeadzone = -0.9f;
    const float rightTriggerDeadzone = -0.9f;
    // Get axis values
    float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
    float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
    float rightStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
    float rightStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
    // Calculate deadzones
    if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
    if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
    if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
    if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;

    int pressed = false;
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP) || -leftStickY > leftStickDeadzoneY){
        movePlayer(player, GetFrameTime());
        pressed = true;
    }
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || leftStickY > leftStickDeadzoneY){
        movePlayer(player, -GetFrameTime());
        pressed = true;
    }
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || -leftStickX > leftStickDeadzoneX){
        rotatePlayer(player, -GetFrameTime());
        pressed = true;
    }
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || leftStickX > leftStickDeadzoneX){
        rotatePlayer(player, GetFrameTime());
        pressed = true;
    }
    return pressed;
}

void updatePlayer(Player *player){
    if(player->timer > 0){
        player->timer -= GetFrameTime();
    }
    for(int i=0; i < player->shots->size; i++){
        if(player->shots->data[i].timer > 0){
            player->shots->data[i].timer -= GetFrameTime();
        }
        else if(player->shots->data[i].timer <= 0){
            //remove shot
            popShot(player->shots);
        }
    }
    
    
    int pressed = false;
    if(IsGamepadAvailable(0)){
        pressed = getGamepadEvents(0, player);
    }
    if(!pressed) {
        if(IsKeyDown(KEY_W)){
            movePlayer(player, GetFrameTime());
        }
        if(IsKeyDown(KEY_S)){
            movePlayer(player, -GetFrameTime());
        }
        if(IsKeyDown(KEY_A)){
            rotatePlayer(player, -GetFrameTime());
        }
        if(IsKeyDown(KEY_D)){
            rotatePlayer(player, GetFrameTime());
        }
        if(IsKeyDown(KEY_SPACE) && player->timer <= 0){
            //shoot
            shoot(player, GetFrameTime());
            // printShotsArray(player->shots);
        }
    }
}