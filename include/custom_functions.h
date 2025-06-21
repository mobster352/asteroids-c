#include "raylib.h"
#include "raymath.h"

#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

void* concatIntToString(const char *string, int value); // Function declaration
void* concatStringToString(const char *string, const char *value);
void* concatVector2ToString(const char *string, Vector2 value);
Vector2 checkOffScreen(Vector2 position, int unitsOffScreen);
float generate_random_float(float min, float max);

#endif