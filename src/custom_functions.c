#include "raylib.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "custom_functions.h"

void* concatIntToString(const char *string, int value){
	char buffer[16]; //safe size for integers - 12 bytes for most ints and 1 more for the null terminator
	//this makes sure that the value can fit inside of the buffer
	int bufferSize = snprintf(buffer, sizeof(buffer), "%d", value);
	//Calculate required size for new string
	size_t new_string_len = strlen(string) + strlen(buffer) + 1; //+1 for null terminator
	//Allocate memory for new string
	char *result = (char *)malloc(new_string_len);
	if(result == NULL){
		perror("Failed to allocate memory");
		return NULL;
	}
	//Copy the first string
	strcpy(result, string);
	//Concatenate the second string
	strcat(result, buffer);
	return result;
}

void* concatVector2ToString(const char *string, Vector2 value){
	char buffer[32];
	// Format the vector as (x, y)
    snprintf(buffer, sizeof(buffer), "(%d, %d)", (int)value.x, (int)value.y);
    // Calculate required size for new string
	//Calculate required size for new string
	size_t new_string_len = strlen(string) + strlen(buffer) + 1; //+1 for null terminator
	//Allocate memory for new string
	char *result = (char *)malloc(new_string_len);
	if(result == NULL){
		perror("Failed to allocate memory");
		return NULL;
	}
	//Copy the first string
	strcpy(result, string);
	strcat(result, buffer);
	return result;
}

void* concatStringToString(const char *string, const char *value){
	//Calculate required size for new string
	size_t new_string_len = strlen(string) + strlen(value) + 1; //+1 for null terminator
	//Allocate memory for new string
	char *result = (char *)malloc(new_string_len);
	if(result == NULL){
		perror("Failed to allocate memory");
		return NULL;
	}
	//Copy the first string
	strcpy(result, string);
	//Concatenate the second string
	strcat(result, value);
	return result;
}

Vector2 checkOffScreen(Vector2 position, int unitsOffScreen){
    if(position.x >= GetScreenWidth() + unitsOffScreen)
        position.x = -unitsOffScreen;
    else if(position.x <= -unitsOffScreen)
        position.x = GetScreenWidth();
    else if(position.y <= -unitsOffScreen)
        position.y = GetScreenHeight();
    else if(position.y >= GetScreenHeight() + unitsOffScreen)
        position.y = -unitsOffScreen;
    return position;
}