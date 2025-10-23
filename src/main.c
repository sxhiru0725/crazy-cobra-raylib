#include "raylib.h"
#include <stdlib.h>
#include <math.h>

int main() {
    const int screenWidth = 960;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Crazy Cobra");
    SetTargetFPS(30);

    // audio
    InitAudioDevice();
    Sound eatSound = LoadSound("resources/eat_sound.wav");
    Sound gameOverSound = LoadSound("resources/game_over_sound.wav");

    // snake
    int x = 260, y = 300;
    int speed = 10;
    int length = 1;
    int dx = speed, dy = 0;
    int snakeBody[400][2];

    // food
    int FoodX = 100, FoodY = 100;
    
    //screens
    bool gameOver = false;
    bool gameStarted = false;
    bool paused = false;

    int score = 0;
    int health = 3;
    int frameCounter = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();

        // background
        if (gameOver) {
            Color flash = (frameCounter / 30) % 2 == 0 ? RED : DARKGRAY;
            ClearBackground(flash);
        } else if (gameStarted) {
            ClearBackground((Color){180, 238, 180, 255});
            DrawRectangleLines(0, 0, screenWidth - 1, screenHeight - 1, DARKGREEN);
        }

        // start screen
        if (!gameStarted) {
            Color flash = (frameCounter / 30) % 2 == 0 ? GREEN : GRAY;
            ClearBackground(flash);
            DrawText("CRAZY COBRA", screenWidth / 2 - 170, screenHeight / 2 - 60, 40, WHITE);
            DrawText("Press ENTER to Start", screenWidth / 2 - 170, screenHeight / 2 + 10, 30, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                gameStarted = true;
            }

            frameCounter++;
            EndDrawing();
            continue;
        }

        // pause
        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (paused) {
            DrawText("PAUSED", screenWidth / 2 - 140, screenHeight / 2 - 40, 40, BLACK);
            DrawText("Press 'P' to Continue", screenWidth / 2 - 200, screenHeight / 2 + 20, 30, RED);
            EndDrawing();
            continue;
        }

        // gameover screen
        if (gameOver) {
            PlaySound(gameOverSound);
            DrawText("GAME OVER!", screenWidth / 2 - 140, screenHeight / 2 - 40, 40, BLACK);
            if ((frameCounter / 20) % 2 == 0) {
                DrawText("Press ENTER to Restart", screenWidth / 2 - 200, screenHeight / 2 + 20, 30, WHITE);
            }
            frameCounter++;

            if (IsKeyPressed(KEY_ENTER)) {
                x = 260; y = 300; dx = speed; dy = 0;
                length = 1; health = 3; speed = 10; score = 0;
                gameOver = false;
                FoodX = GetRandomValue(0, 47) * 20;
                FoodY = GetRandomValue(0, 35) * 20;
            }

            EndDrawing();
            continue;
        }

        // controls
        if (IsKeyPressed(KEY_UP) && dy != speed) { dx = 0; dy = -speed; }
        if (IsKeyPressed(KEY_DOWN) && dy != -speed) { dx = 0; dy = speed; }
        if (IsKeyPressed(KEY_LEFT) && dx != speed) { dx = -speed; dy = 0; }
        if (IsKeyPressed(KEY_RIGHT) && dx != -speed) { dx = speed; dy = 0; }

        // move
        x += dx; y += dy;

        // wall collision
        if (x < 0 || y < 0 || x >= screenWidth || y >= screenHeight) {
            health--;
            if (health <= 0) {
                gameOver = true;
            } else {
                x = 260; y = 300; dx = speed; dy = 0;
                length = 1;
                FoodX = GetRandomValue(0, 47) * 20;
                FoodY = GetRandomValue(0, 35) * 20;
            }
        }

        // self collision
        for (int i = 1; i < length; i++) {
            if (x == snakeBody[i][0] && y == snakeBody[i][1]) {
                health--;
                if (health <= 0) {
                    gameOver = true;
                } else {
                    x = 260; y = 300; dx = speed; dy = 0;
                    length = 1;
                    FoodX = GetRandomValue(0, 47) * 20;
                    FoodY = GetRandomValue(0, 35) * 20;
                }
            }
        }

        // eat food
        if (abs(x - FoodX) < 20 && abs(y - FoodY) < 20) {
            PlaySound(eatSound);
            do {
                FoodX = GetRandomValue(0, 47) * 20;
                FoodY = GetRandomValue(0, 35) * 20;
            } while (x == FoodX && y == FoodY);

            length++;
            score++;

            if (score % 5 == 0 && speed < 30) {
                speed += 2;
                if (dx != 0) dx = (dx > 0) ? speed : -speed;
                if (dy != 0) dy = (dy > 0) ? speed : -speed;
            }
        }

        // snake body movement
        for (int i = length - 1; i > 0; i--) {
            snakeBody[i][0] = snakeBody[i - 1][0];
            snakeBody[i][1] = snakeBody[i - 1][1];
        }
        snakeBody[0][0] = x;
        snakeBody[0][1] = y;

        // snake
        for (int i = 0; i < length; i++) {
            DrawRectangle(snakeBody[i][0], snakeBody[i][1], 20, 20, DARKGREEN);
            DrawRectangleLines(snakeBody[i][0], snakeBody[i][1], 20, 20, BLACK);
        }

        // food
        DrawRectangle(FoodX, FoodY, 20, 20, RED);

        // upper bar
        DrawText(TextFormat("Score: %d", score), 10, 10, 25, GOLD);
        DrawText(TextFormat("Health: %d", health), 10, 40, 25, SKYBLUE);
        DrawText("Press 'P' to Pause", 700, 10, 25, WHITE);

        if (score % 5 == 0 && score != 0) {
            DrawText("Speed Up!", screenWidth / 2 - 60, screenHeight - 40, 20, ORANGE);
        }

        EndDrawing();
    }

    UnloadSound(eatSound);
    UnloadSound(gameOverSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
