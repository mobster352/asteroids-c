#include "raylib.h"
#include "raymath.h"

#ifndef CIRCLE_SHAPE_H
#define CIRCLE_SHAPE_H

typedef struct circleShape{
    Vector2 position;
    float radius;
    Color color;
    Vector2 velocity;
} CircleShape;

typedef struct shot{
    int id;
    CircleShape shape;
    float timer;
} Shot;

typedef struct shotsArray{
    Shot *data;
    int capacity;
    int size;
} ShotsArray;

typedef struct player{
    CircleShape shape;
    float rotation;
    ShotsArray* shots;
    int shotCount;
    float timer;
} Player;

typedef struct asteroid{
    CircleShape shape;
    int id;
} Asteroid;

typedef struct edge{
    Vector2 velocity;
    Vector2 position;
} Edge;

typedef struct asteroidsArray{
    Asteroid* data;
    int capacity;
    int size;
} AsteroidsArray;

typedef struct asteroidField{
    float spawnTimer;
    Edge* edges;
    AsteroidsArray* asteroids;
    int asteroidId;
} AsteroidField;

void createPlayer(Player *p);
void drawPlayer(Player player, bool hasShield, float iFrames);
void updatePlayer(Player *player);

ShotsArray* createShotsArray(int capacity);
void insertShot(ShotsArray* arr, Shot element);
int removeShotById(ShotsArray* arr, int shotId);
void resizeShotsArray(ShotsArray* arr, int newCapacity);
void freeShotsArray(ShotsArray* arr);
Shot getShotFromArray(ShotsArray* arr, int shotId);
int findShotFromArray(ShotsArray* arr, int shotId);
void printShotsArray(ShotsArray* arr);
int popShot(ShotsArray* arr);
void drawShotsArray(ShotsArray* arr);

AsteroidsArray* createAsteroidsArray(int capacity);
void resizeAsteroidsArray(AsteroidsArray* arr, int newCapacity);
void insertAsteroid(AsteroidsArray* arr, Asteroid element);
int removeAsteroidById(AsteroidsArray* arr, int AsteroidId);
int popAsteroid(AsteroidsArray* arr);
Asteroid getAsteroidFromArray(AsteroidsArray* arr, int AsteroidId);
int findAsteroidFromArray(AsteroidsArray* arr, int AsteroidId);
void freeAsteroidsArray(AsteroidsArray* arr);
void printAsteroidsArray(AsteroidsArray* arr);
void drawAsteroidsArray(AsteroidsArray* arr);

Asteroid createAsteroid(Vector2 position, Vector2 velocity, int radius, int id, Color color);
void splitAsteroid(AsteroidField *field, int id);

void createAsteroidField(AsteroidField *field);
void updateAsteroidField(AsteroidField *field);

#endif