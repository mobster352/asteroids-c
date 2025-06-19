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
#include "player.h"

// void createPlayer(Player *p){
// 	const int playerRadius = 20;
// 	CircleShape shape = {
// 		{500, 500}, playerRadius, WHITE, {0, 0}
// 	};

// 	p->shape = shape;
// 	p->rotation = 0.0;
// 	// printf("player.position: {%.2f, %.2f}\n", p->shape.position.x, p->shape.position.y);
// 	// printf("player.rotation: %.2f\n", p->rotation);
// }

int main (){
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_UNDECORATED);

	// ToggleBorderlessWindowed();

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");
	// InitWindow(GetScreenWidth(), GetScreenHeight(), "Game");

	SetExitKey(KEY_NULL); //this disables ESC to close the game

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	SetTargetFPS(120);

	Player player;
	createPlayer(&player);
	// printf("player.position: {%.2f, %.2f}\n", player.shape.position.x, player.shape.position.y);
	// printf("player.rotation: %.2f\n", player.rotation);
	
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

		// draw some text using the default font
		// const char *text = "Value:";
		// int value = 250;
		// const char *newText = concatIntToString(text, value);
		// DrawText(newText, 10,10,20,WHITE);

		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);

		updatePlayer(&player);
		drawPlayer(player);

		// run = false;
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}