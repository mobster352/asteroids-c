#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "custom_functions.h"
#include "circle_shape.h"

typedef enum {
	GAME_STATE_MENU,
	GAME_STATE_PLAYING,
	GAME_STATE_PAUSED
} GameState;

const int BUTTON_WIDTH = 250;
const int BUTTON_HEIGHT = 60;
const int BUTTON_MARGIN = 10;
const int BUTTON_Y = 400;
const int BUTTON_SPACING = 30;      // Space between buttons

void drawTextWithInt(char *text, int value, int posX, int posY, int fontSize, Color color){
	void *newText = concatIntToString(text, value);
	DrawText(newText,posX,posY,fontSize,color);
	free(newText);
}

void drawTextWithVector2(char *text, Vector2 value, int posX, int posY, int fontSize, Color color){
	void *newText = concatVector2ToString(text, value);
	DrawText(newText,posX,posY,fontSize,color);
	free(newText);
}

void drawTextWithString(char *text, const char *value, int posX, int posY, int fontSize, Color color){
	void *newText = concatStringToString(text, value);
	DrawText(newText,posX,posY,fontSize,color);
	free(newText);
}

// Helper function to draw a centered button with text and margin
bool DrawCenteredButton(const char *text, int centerX, int centerY, int width, int height, int margin, Color textColor) {	
	// Rectangle centered at (centerX, centerY)
    Rectangle rect = {
        centerX - width / 2,
        centerY - height / 2,
        width,
        height
    };

	Color idleColor = GRAY;
    Color hoverColor = BLUE;
	Color btnColor = CheckCollisionPointRec(GetMousePosition(), rect) ? hoverColor : idleColor;
    // Draw rectangle
    DrawRectangleRec(rect, btnColor);

    // Measure text
    int textWidth = MeasureText(text, height - 2 * margin);
    int textHeight = height - 2 * margin;

    // Calculate text position (centered inside rectangle with margin)
    int textX = centerX - textWidth / 2;
    int textY = centerY - textHeight / 2;

    // Draw text
    DrawText(text, textX, textY, textHeight, textColor);

    // Check for click
    return CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void initGame(Player *p, AsteroidField *f){
	createPlayer(p);
	createAsteroidField(f);
}

void leaveGame(Player *p, AsteroidField *f){
	p = NULL; //set ptr to NULL to free it
	f = NULL;
}

void mainMenu(GameState *state, bool *run, Player *player, AsteroidField *field){
	DrawText("Asteroids", GetScreenWidth()/2 - MeasureText("Asteroids", 75)/2, 150, 75, WHITE);
	int n = 1; // number of additional buttons // n = 0 for only 1 button
    if (DrawCenteredButton("New Game", GetScreenWidth()/2, BUTTON_Y + BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN, WHITE)) {
        *state = GAME_STATE_PLAYING;
		initGame(player, field);
    }
	else if (DrawCenteredButton("Quit Game", GetScreenWidth()/2, BUTTON_Y + BUTTON_HEIGHT/2 + n*(BUTTON_HEIGHT + BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN, WHITE)) {
        *run = false;
    }
}

void pauseMenu(GameState *state){
    DrawText("Paused", GetScreenWidth()/2 - MeasureText("Paused", 75)/2, 150, 75, WHITE);
    if (DrawCenteredButton("Leave Game", GetScreenWidth()/2, BUTTON_Y + BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN, WHITE)) {
        *state = GAME_STATE_MENU;
    }
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
	Texture background = LoadTexture("space3.jpg");
	SetTargetFPS(60);
	SetWindowFocused();

	Player player;
	AsteroidField asteroidField;
	GameState state = GAME_STATE_MENU;

	bool run = true;
	// game loop
	while (run){		// run the loop untill the user presses ESCAPE or presses the Close button on the window
		if(IsKeyPressed(KEY_F4)){
			run = false;
		}
		if(IsKeyPressed(KEY_ESCAPE) && (state == GAME_STATE_PLAYING || state == GAME_STATE_PAUSED)){
			if(state == GAME_STATE_PLAYING)
				state = GAME_STATE_PAUSED;
			else
				state = GAME_STATE_PLAYING;
		}

		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		DrawTexture(background, 0, 0, WHITE);
		// draw FPS
		drawTextWithInt("FPS: ", GetFPS(), 1180, 10, 20, WHITE);

		// draw HUD
		drawTextWithInt("Score: ", 0, 10, 10, 20, WHITE);

		//draw controller
		if(IsGamepadAvailable(0)){
			drawTextWithString("Controller: ", GetGamepadName(0), 10, 30, 20, WHITE);
		}

		drawTextWithVector2("Pos: ", GetMousePosition(), 1100, 780, 20, WHITE);

		if(state == GAME_STATE_PAUSED){
			pauseMenu(&state);
			// leave game
			if(state == GAME_STATE_MENU){
				leaveGame(&player, &asteroidField);
			}
		}	
		else if(state == GAME_STATE_MENU){
			mainMenu(&state, &run, &player, &asteroidField);
		}
		else if(state == GAME_STATE_PLAYING){
			updateAsteroidField(&asteroidField);
			drawAsteroidsArray(asteroidField.asteroids);

			updatePlayer(&player);
			drawPlayer(player);

			//update shots
			drawShotsArray(player.shots);
		}
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(background);
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}