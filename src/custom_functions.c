#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "custom_functions.h"

const char* concatIntToString(const char *string, int value){
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

const char* concatStringToString(const char *string, const char *value){
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