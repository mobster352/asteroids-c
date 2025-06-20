#include "stdlib.h"
#include "stdio.h"
#include "circle_shape.h"

ShotsArray* createShotsArray(int capacity){
    ShotsArray* arr = (ShotsArray*)malloc(sizeof(ShotsArray));
    arr->data = (Shot*)malloc(capacity * sizeof(Shot));
    arr->capacity = capacity;
    arr->size = 0;
    return arr;
}

void resizeShotsArray(ShotsArray* arr, int newCapacity){
    Shot* newData = (Shot*)malloc(newCapacity * sizeof(Shot));
    for (int i = 0; i < arr->size; i++) {
        newData[i] = arr->data[i];
    }
    free(arr->data);
    arr->data = newData;
    arr->capacity = newCapacity;
}

void insertShot(ShotsArray* arr, Shot element){
    if (arr->size == arr->capacity) {
        resizeShotsArray(arr, arr->capacity * 2);
    }
    arr->data[arr->size] = element;
    arr->size++;
}

int removeShotById(ShotsArray* arr, int shotId){
    if (arr->size == 0) {
        printf("Error: Array is empty\n");
        return -1;
    }
    int shotIndex = findShotFromArray(arr, shotId);
    if(shotIndex != -1){
        //shift elements left after index
        for(int i=shotIndex; i < arr->size; i++){
            if(i+1 < arr->size)
                arr->data[i] = arr->data[i+1];
        }
    }
    else{
        return -1;
    }
    arr->size--;
    if (arr->size < arr->capacity / 4) {
        resizeShotsArray(arr, arr->capacity / 2);
    }
    return 0;
}

int popShot(ShotsArray* arr){
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
        resizeShotsArray(arr, arr->capacity / 2);
    }
    return 0;
}

Shot getShotFromArray(ShotsArray* arr, int shotId){
    for(int i = 0; i < arr->size; i++){
        if(arr->data[i].id == shotId){
            return arr->data[i];
        }
    }
}

int findShotFromArray(ShotsArray* arr, int shotId){
    for(int i = 0; i < arr->size; i++){
        if(arr->data[i].id == shotId){
            return i;
        }
    }
    return -1;
}

void freeShotsArray(ShotsArray* arr){
    free(arr->data);
    free(arr);
}

void printShotsArray(ShotsArray* arr){
    printf("Printing ShotsArray...\n");
    printf("arr.size: %d\n", arr->size);
    for(int i=0; i < arr->size; i++){
        printf("shot id: %d\n", arr->data[i].id);
    }
    printf("End Print...\n");
}

void drawShotsArray(ShotsArray* arr){
    for(int i=0; i < arr->size; i++){
        arr->data[i].shape.position = Vector2Add(arr->data[i].shape.position, Vector2Scale(arr->data[i].shape.velocity, GetFrameTime()));
        DrawCircle(arr->data[i].shape.position.x, arr->data[i].shape.position.y, arr->data[i].shape.radius, arr->data[i].shape.color);
    }
}