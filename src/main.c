/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "custom_functions.h"
#include "circle_shape.h"

void drawTextWithInt(char *text, int value, int posX, int posY, int fontSize, Color color){
	const char *newText = concatIntToString(text, value);
	DrawText(newText,posX,posY,fontSize,color);
}

void drawTextWithString(char *text, const char *value, int posX, int posY, int fontSize, Color color){
	const char *newText = concatStringToString(text, value);
	DrawText(newText,posX,posY,fontSize,color);
}

int main (){
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_UNDECORATED);
	// ToggleBorderlessWindowed();
	// Create the window and OpenGL context
	InitWindow(1280, 800, "Asteroids");
	// InitWindow(GetScreenWidth(), GetScreenHeight(), "Game");
	SetExitKey(KEY_NULL); //this disables ESC to close the game
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	// Load a texture from the resources directory
	// Texture wabbit = LoadTexture("wabbit_alpha.png");
	SetTargetFPS(60);

	Player player;
	createPlayer(&player);

	bool run = true;
	// game loop
	while (run){		// run the loop untill the user presses ESCAPE or presses the Close button on the window
		if(IsKeyPressed(KEY_F4)){
			run = false;
		}
		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);
		// draw FPS
		drawTextWithInt("FPS: ", GetFPS(), 10, 10, 20, WHITE);
		//draw controller
		if(IsGamepadAvailable(0)){
			drawTextWithString("Controller: ", GetGamepadName(0), 10, 30, 20, WHITE);
		}
		updatePlayer(&player);
		drawPlayer(player);

		//update shots
		drawShotsArray(player.shots);
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	// cleanup
	// unload our texture so it can be cleaned up
	// UnloadTexture(wabbit);
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}