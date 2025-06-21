#include "circle_shape.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

const int ASTEROID_MIN_RADIUS = 20;
const int ASTEROID_KINDS = 3;
const float ASTEROID_SPAWN_RATE = 0.8f;  // seconds
const int ASTEROID_MAX_RADIUS = ASTEROID_MIN_RADIUS * ASTEROID_KINDS;
const float ASTEROID_ROTATE_RADS = 0.523599f;

Vector2 getEdge(float scaler, Vector2 (*getVector2)(float)){
    return getVector2(scaler);
}

Vector2 getEdge1(float scaler){
    return (Vector2){-ASTEROID_MAX_RADIUS, scaler * GetScreenHeight()};
}

Vector2 getEdge2(float scaler){
    return (Vector2){GetScreenWidth() + ASTEROID_MAX_RADIUS, scaler * GetScreenHeight()};
}

Vector2 getEdge3(float scaler){
    return (Vector2){scaler * GetScreenWidth(), -ASTEROID_MAX_RADIUS};
}

Vector2 getEdge4(float scaler){
    return (Vector2){scaler * GetScreenWidth(), GetScreenHeight() + ASTEROID_MAX_RADIUS};
}

void createAsteroidField(AsteroidField *field){
    field->edges = malloc(4 * sizeof(Edge));
    field->edges[0] = (Edge){ (Vector2){1,0}, (Vector2){0,0} };
    field->edges[1] = (Edge){ (Vector2){-1,0}, (Vector2){0,0} };
    field->edges[2] = (Edge){ (Vector2){0,1}, (Vector2){0,0} };
    field->edges[3] = (Edge){ (Vector2){0,-1}, (Vector2){0,0} };

	field->asteroidId = 0;
    field->spawnTimer = 0;
    field->asteroids = createAsteroidsArray(1);

    srand(time(NULL));
}

void spawn(AsteroidField *field, int radius, Edge edge){
    Color colorOptions[] = {WHITE};
    int numColorOptions = sizeof(colorOptions) / sizeof(colorOptions[0]);
    int colorIndex = rand() % numColorOptions;
    Asteroid a = createAsteroid(edge.position, edge.velocity, radius, field->asteroidId, colorOptions[colorIndex]);
    insertAsteroid(field->asteroids, a);
    field->asteroidId++;
}

// Function to generate a random float within a specified range
float generate_random_float(float min, float max) {
    float scale = (float)rand() / RAND_MAX;
    return min + scale * (max - min);
}

void updateAsteroidField(AsteroidField *field){
    field->spawnTimer += GetFrameTime();
    if(field->spawnTimer > ASTEROID_SPAWN_RATE){
        field->spawnTimer = 0;
        if(field->asteroids->size < 40){
            int random_index = rand() % 4;
            Edge edge = field->edges[random_index];

            int speed = (rand() % (100 - 40 + 1)) + 40;
            edge.velocity = Vector2Scale(edge.velocity, speed);
            edge.velocity = Vector2Rotate(edge.velocity, generate_random_float(-ASTEROID_ROTATE_RADS, ASTEROID_ROTATE_RADS));

            if(random_index == 0)
                edge.position = getEdge(generate_random_float(0, 1), getEdge1);
            if(random_index == 1)
                edge.position = getEdge(generate_random_float(0, 1), getEdge2);
            if(random_index == 2)
                edge.position = getEdge(generate_random_float(0, 1), getEdge3);
            if(random_index == 3)
                edge.position = getEdge(generate_random_float(0, 1), getEdge4);

            int kind = (rand() % (ASTEROID_KINDS)) + 1;
            spawn(field, ASTEROID_MIN_RADIUS * kind, edge);
        }
    }
}