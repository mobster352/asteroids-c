#include "stdlib.h"
#include "stdio.h"
#include "circle_shape.h"
#include "custom_functions.h"

AsteroidsArray* createAsteroidsArray(int capacity){
    AsteroidsArray* arr = (AsteroidsArray*)malloc(sizeof(AsteroidsArray));
    arr->data = (Asteroid*)malloc(capacity * sizeof(Asteroid));
    arr->capacity = capacity;
    arr->size = 0;
    return arr;
}

void resizeAsteroidsArray(AsteroidsArray* arr, int newCapacity){
    Asteroid* newData = (Asteroid*)malloc(newCapacity * sizeof(Asteroid));
    for (int i = 0; i < arr->size; i++) {
        newData[i] = arr->data[i];
    }
    free(arr->data);
    arr->data = newData;
    arr->capacity = newCapacity;
}

void insertAsteroid(AsteroidsArray* arr, Asteroid element){
    if (arr->size == arr->capacity) {
        resizeAsteroidsArray(arr, arr->capacity * 2);
    }
    arr->data[arr->size] = element;
    arr->size++;
}

int removeAsteroidById(AsteroidsArray* arr, int AsteroidId){
    if (arr->size == 0) {
        printf("Error: Array is empty\n");
        return -1;
    }
    int AsteroidIndex = findAsteroidFromArray(arr, AsteroidId);
    if(AsteroidIndex != -1){
        //shift elements left after index
        for(int i=AsteroidIndex; i < arr->size; i++){
            if(i+1 < arr->size)
                arr->data[i] = arr->data[i+1];
        }
    }
    else{
        return -1;
    }
    arr->size--;
    if (arr->size < arr->capacity / 4) {
        resizeAsteroidsArray(arr, arr->capacity / 2);
    }
    return 0;
}

int popAsteroid(AsteroidsArray* arr){
    if (arr->size == 0) {
        printf("Error: Array is empty\n");
        return -1;
    }
    for(int i=0; i < arr->size; i++){
        if(i+1 < arr->size){
            arr->data[i] = arr->data[i+1];
        }
    }
    arr->size--;
    if (arr->size < arr->capacity / 4) {
        resizeAsteroidsArray(arr, arr->capacity / 2);
    }
    return 0;
}

Asteroid getAsteroidFromArray(AsteroidsArray* arr, int AsteroidId){
    for(int i = 0; i < arr->size; i++){
        if(arr->data[i].id == AsteroidId){
            return arr->data[i];
        }
    }
}

int findAsteroidFromArray(AsteroidsArray* arr, int AsteroidId){
    for(int i = 0; i < arr->size; i++){
        if(arr->data[i].id == AsteroidId){
            return i;
        }
    }
    return -1;
}

void freeAsteroidsArray(AsteroidsArray* arr){
    free(arr->data);
    free(arr);
}

void printAsteroidsArray(AsteroidsArray* arr){
    printf("Printing AsteroidsArray...\n");
    printf("arr.size: %d\n", arr->size);
    for(int i=0; i < arr->size; i++){
        printf("Asteroid id: %d\n", arr->data[i].id);
    }
    printf("End Print...\n");
}

void drawAsteroidsArray(AsteroidsArray* arr){
    for(int i=0; i < arr->size; i++){
        arr->data[i].shape.position = Vector2Add(arr->data[i].shape.position, Vector2Scale(arr->data[i].shape.velocity, GetFrameTime()));
        arr->data[i].shape.position = checkOffScreen(arr->data[i].shape.position, arr->data->shape.radius*2);
        DrawCircleLines(arr->data[i].shape.position.x, arr->data[i].shape.position.y, arr->data[i].shape.radius, arr->data[i].shape.color);
        // printf("Asteroid: %d: Pos: (%.2f, %.2f), Vel: (%.2f, %.2f)\n", arr->data[i].id, arr->data[i].shape.position.x, arr->data[i].shape.position.y, arr->data[i].shape.velocity.x, arr->data[i].shape.velocity.y);
    }
}