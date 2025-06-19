#include "raylib.h"

#ifndef CIRCLE_SHAPE_H
#define CIRCLE_SHAPE_H

typedef struct circleShape{
    Vector2 position;
    float radius;
    Color color;
    Vector2 velocity;
} CircleShape;

typedef struct player{
    CircleShape shape;
    float rotation;
} Player;

#endif