#include "player.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"

const int PLAYER_RADIUS = 20;
const int PLAYER_TURN_SPEED = 5;
const int PLAYER_SPEED = 10;

void createPlayer(Player *p){
	CircleShape shape = {
		{500, 500}, PLAYER_RADIUS, WHITE, {0, 0}
	};

	p->shape = shape;
	p->rotation = 0.0;
	// printf("player.position: {%.2f, %.2f}\n", p->shape.position.x, p->shape.position.y);
	// printf("player.rotation: %.2f\n", p->rotation);
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
    // printf("vertices[0]: (%.2f, %.2f),\nvertices[1]: (%.2f, %.2f),\nvertices[2]: (%.2f, %.2f)\n", vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y, vertices[2].x, vertices[2].y);
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
    player->shape.position = Vector2Add(player->shape.position, Vector2Scale(forward, PLAYER_SPEED * dt));
}

void updatePlayer(Player *player){
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
}