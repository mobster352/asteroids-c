#include "circle_shape.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"

Asteroid createAsteroid(Vector2 position, Vector2 velocity, int radius, int id, Color color){
	Asteroid a;
    CircleShape shape = {
		position, radius, color, velocity
	};
	a.shape = shape;
	a.id = id;
    return a;
}