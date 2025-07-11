#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "SignUpScreen.h"
#include "LoginScreen.h"
#include "SplashScreen.h"
#include "home_screen.h"
#include "sweep.h"
#include"leader_board.h"

using namespace std;

// External variables
extern UIManager ui;
extern Font gameFont;

extern const int screen_width;
extern const int screen_height;

extern const char *labelGameWin;
extern const char *labelGameLose;
extern const char *labelEnter;

extern float timeGameEnded;
extern float timeGameStarted;

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

enum class Screen {
    SPLASH,
    SIGNUP,
    LOGIN,
    HOME,
    GAMEPLAY
};

int main()
{
    InitWindow(screen_width, screen_height, "Minesweeper");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    SignUpScreen signupScreen(screen_width, screen_height);
    LoginScreen loginScreen(screen_width, screen_height);
    SplashScreen splashScreen;
 std::string url = "http://localhost:18080/playersInfo";  // Adjust to your actual GET endpoint
    std::vector<PlayerData> players = LeaderBoard::fetchTopPlayers(url);

    for (const auto& player : players) {
              std::cout << "UserName     : " << player.user_name << "\n";
        std::cout << "Email     : " << player.email << "\n";
        std::cout << "Best Time : " << player.bestTime/60<<" min"<<":"<<player.bestTime%60 <<" second"<< "\n";
        std::cout << "Wins      : " << player.won << "\n";
        std::cout << "Losses    : " << player.lose << "\n";
        std::cout << "--------------------------\n";
    }



    UIManager homeScreen;
    Game game;

    homeScreen.Init();
    game.GameStartup();

    Screen currentScreen = Screen::SPLASH;

    while (!WindowShouldClose())
    {
        if (currentScreen != Screen::GAMEPLAY)
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
        }

        switch (currentScreen)
        {
            case Screen::SPLASH:
                splashScreen.Update();
                splashScreen.Draw();
                if (splashScreen.IsFinished()) currentScreen = Screen::SIGNUP;
                break;

            case Screen::SIGNUP:
                signupScreen.Update();
                signupScreen.Draw();
                if (signupScreen.IsSignInClicked()) {
                    signupScreen.ResetSignInClick();
                    currentScreen = Screen::LOGIN;
                } else if (signupScreen.IsAccountCreated()) {
                    signupScreen.ResetAccountCreated();
                    currentScreen = Screen::LOGIN;
                }
                break;

            case Screen::LOGIN:
                loginScreen.Update();
                loginScreen.Draw();
                if (loginScreen.IsRegisterClicked()) {
                    loginScreen.ResetRegisterClick();
                    currentScreen = Screen::SIGNUP;
                } else if (loginScreen.IsLoggedIn()) {
                    loginScreen.ResetLoggedIn();
                    currentScreen = Screen::GAMEPLAY;
                }
                break;  // ✅ FIXED: Prevent fallthrough to GAMEPLAY

            case Screen::GAMEPLAY:
            {
                game.GameUpdate();

                BeginDrawing();
                ClearBackground(RAYWHITE);
                game.GameRender();
                EndDrawing();

                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentScreen = Screen::HOME;
                }
                break;
            }
        }

        if (currentScreen != Screen::GAMEPLAY) {
            EndDrawing();
        }
    }

    // ✅ FIXED: These lines must be *after* the main loop
    game.GameShutdown();
    CloseWindow();

    return 0;
}
