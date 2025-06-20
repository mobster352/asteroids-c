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

void createPlayer(Player *p);
void drawPlayer(Player player);
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

#endif