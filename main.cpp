#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include "home_screen.h"
#include "sweep.h"
using namespace std;

// This is calling strcture variable from home_screen.cpp
extern UIManager ui;

extern Font gameFont;

// Constants defining the grid size and screen resolution
extern const int screen_width;
extern const int screen_height;

// Labels for different game states
extern const char *labelGameWin;
extern const char *labelGameLose;
extern const char *labelEnter;

// Time tracking variables
extern float timeGameEnded;
extern float timeGameStarted;

// Sound Variables
extern bool isSoundEnabled;
extern bool isMusicEnabled;

extern Difficulty easy;
extern Difficulty medium;
extern Difficulty hard;

extern Texture2D textures[MAX_TEXTURES];
extern Sound sounds[MAX_SOUNDS];
extern Music music[MAX_MUSIC];

extern int revealedTilesCount;
extern int minesPresentCount;

extern game_states gameState;

int main()
{


    InitWindow(screen_width, screen_height, "Minesweeper by Prasiddha");
    SetExitKey(KEY_NULL);
    Game game;
    game.GameStartup();


    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        game.GameUpdate();


        BeginDrawing();
        ClearBackground(RAYWHITE);


        game.GameRender();


        EndDrawing();
    }


    game.GameShutdown();


    return 0;
}