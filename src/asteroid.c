#include "circle_shape.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"
#include "custom_functions.h"
#include "constants.h"

Asteroid createAsteroid(Vector2 position, Vector2 velocity, int radius, int id, Color color){
	Asteroid a;
    CircleShape shape = {
		position, radius, color, velocity
	};
	a.shape = shape;
	a.id = id;
    return a;
}

void splitAsteroid(AsteroidField *field, int i){
    if(field->asteroids->data[i].shape.radius <= ASTEROID_MIN_RADIUS){
        //smallest asteroid
    } else{
        //split
        float angle = generate_random_float(ASTEROID_ROTATE_RADS_MIN, ASTEROID_ROTATE_RADS_MAX);
        Vector2 vector1 = Vector2Rotate(field->asteroids->data[i].shape.velocity, angle);
        Vector2 vector2 = Vector2Rotate(field->asteroids->data[i].shape.velocity, -angle);

        int newRadius = field->asteroids->data[i].shape.radius - ASTEROID_MIN_RADIUS;
        Color colorOptions[] = {WHITE};
        int numColorOptions = sizeof(colorOptions) / sizeof(colorOptions[0]);
        int colorIndex = rand() % numColorOptions;

        Asteroid a;
        a = createAsteroid(field->asteroids->data[i].shape.position, Vector2Scale(vector1, 1.2f), newRadius, field->asteroidId, colorOptions[colorIndex]);
        insertAsteroid(field->asteroids, a);
        field->asteroidId++;
        a = createAsteroid(field->asteroids->data[i].shape.position, Vector2Scale(vector2, 1.2f), newRadius, field->asteroidId, colorOptions[colorIndex]);
        insertAsteroid(field->asteroids, a);
        field->asteroidId++;
    }
    removeAsteroidById(field->asteroids, field->asteroids->data[i].id);
}