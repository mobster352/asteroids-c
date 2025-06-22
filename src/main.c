#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "custom_functions.h"
#include "circle_shape.h"
#include "constants.h"

typedef enum {
	GAME_STATE_MENU,
	GAME_STATE_PLAYING,
	GAME_STATE_PAUSED,
	GAME_STATE_GAME_OVER
} GameState;

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

void initGame(Player *p, AsteroidField *f, int *score, int *highScore, bool *hasShield, int *shieldCounter){
	createPlayer(p);
	createAsteroidField(f);
	*score = 0;
	*hasShield = false;
	*shieldCounter = 0;
}

void leaveGame(Player *p, AsteroidField *f){
	freeShotsArray(p->shots);
	p = NULL; //set ptr to NULL to free it
	freeAsteroidsArray(f->asteroids);
	f = NULL;
}

void mainMenu(GameState *state, bool *run, Player *player, AsteroidField *field, int *score, int *highScore, bool *hasShield, int *shieldCounter){
	DrawText("Asteroids", GetScreenWidth()/2 - MeasureText("Asteroids", 75)/2, 150, 75, WHITE);
	int n = 1; // number of additional buttons // n = 0 for only 1 button
    if (DrawCenteredButton("New Game", GetScreenWidth()/2, BUTTON_Y + BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN, WHITE)) {
        *state = GAME_STATE_PLAYING;
		initGame(player, field, score, highScore, hasShield, shieldCounter);
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

void gameOver(GameState *state, int score, int *highScore){
	DrawText("Game Over!", GetScreenWidth()/2 - MeasureText("Game Over!", 75)/2, 150, 75, WHITE);
	if(score > *highScore){
		char *highScoreText = concatIntToString("New High Score: ", score);
		DrawText(highScoreText, GetScreenWidth()/2 - MeasureText(highScoreText, 50)/2, 250, 50, WHITE);
		free(highScoreText);
	}
	char *scoreText = concatIntToString("Final Score: ", score);
	DrawText(scoreText, GetScreenWidth()/2 - MeasureText(scoreText, 35)/2, 350, 35, WHITE);
	free(scoreText);
    if (DrawCenteredButton("Leave Game", GetScreenWidth()/2, BUTTON_Y + BUTTON_HEIGHT/2, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_MARGIN, WHITE)) {
        *state = GAME_STATE_MENU;
		if(score > *highScore)
			*highScore = score;
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
	int score;
	int highScore = 0;
	bool hasShield;
	int shieldCounter = 0;
	float iFrames = 0.0f;

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
		// drawTextWithInt("FPS: ", GetFPS(), 1180, 10, 20, WHITE);

		//draw controller
		// if(IsGamepadAvailable(0)){
		// 	drawTextWithString("Controller: ", GetGamepadName(0), 10, 30, 20, WHITE);
		// }

		// drawTextWithVector2("Pos: ", GetMousePosition(), 1100, 780, 20, WHITE);

		if(state == GAME_STATE_PAUSED){
			pauseMenu(&state);
			// leave game
			if(state == GAME_STATE_MENU){
				leaveGame(&player, &asteroidField);
			}
		}	
		else if(state == GAME_STATE_MENU){
			mainMenu(&state, &run, &player, &asteroidField, &score, &highScore, &hasShield, &shieldCounter);
		}
		else if(state == GAME_STATE_PLAYING){
			// draw HUD
			drawTextWithInt("Score: ", score, 10, 10, 20, WHITE);
			drawTextWithInt("High Score: ", highScore, 150, 10, 20, WHITE);

			updateAsteroidField(&asteroidField);
			drawAsteroidsArray(asteroidField.asteroids);

			updatePlayer(&player);
			drawPlayer(player, hasShield, iFrames);

			//update shots
			drawShotsArray(player.shots);

			if(iFrames > 0.0f)
				iFrames -= GetFrameTime();

			for(int i=0; i<asteroidField.asteroids->size; i++){
				//check collision with player
				bool hitPlayer = CheckCollisionCircles(
					asteroidField.asteroids->data[i].shape.position, asteroidField.asteroids->data[i].shape.radius,
					player.shape.position, player.shape.radius
				);
				if(hitPlayer && !hasShield && iFrames <= 0.0f){
					state = GAME_STATE_GAME_OVER;
					break;
				} else if(hitPlayer && iFrames <= 0.0f){
					splitAsteroid(&asteroidField, i);
					hasShield = false;
					iFrames = I_FRAMES_COUNTER;
				}

				//check collision with shots
				for(int j=0; j<player.shots->size; j++){
					if(CheckCollisionCircles(
						asteroidField.asteroids->data[i].shape.position, asteroidField.asteroids->data[i].shape.radius,
						player.shots->data[j].shape.position, player.shots->data[j].shape.radius
					)){
						removeShotById(player.shots, player.shots->data[j].id);
						splitAsteroid(&asteroidField, i);
						score += 10;
						shieldCounter++;
						if(shieldCounter >= 10){
							shieldCounter = 0;
							hasShield = true;
						}
					}
				}
			}
		}
		else if(state == GAME_STATE_GAME_OVER){
			gameOver(&state, score, &highScore);
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