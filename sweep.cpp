#include "sweep.h"
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include "home_screen.h"
#include "score_class.h"
#include<iostream>
using namespace std;

// This is calling strcture variable from home_screen.cpp
UIManager ui;

Font gameFont;

// Constants defining the grid size and screen resolution
const int screen_width = 800;
const int screen_height = 970;

// Labels for different game states
const char *labelGameWin = "You Win";
const char *labelGameLose = "Game Over";
const char *labelEnter = "Press ENTER for Main Menu";
string result;
string email;

// Time tracking variables
float timeGameEnded;
float timeGameStarted;

// Split Screen Variables
float loadingTime = 0.0f;
const float loadingDuration = 6.0f; // 4 seconds

// Sound Variables
bool isSoundEnabled = true;
bool isMusicEnabled = true;
bool updated=false;


Difficulty easy = {12, 12, (int)(12 * 12 * 0.1f)};    // 10% mines approx
Difficulty medium = {15, 15, (int)(15 * 15 * 0.15f)}; // ~15% mines
Difficulty hard = {20, 20, (int)(20 * 20 * 0.2f)};    // ~20% mines

Texture2D textures[MAX_TEXTURES];

Sound sounds[MAX_SOUNDS];

Music music[MAX_MUSIC];

game_states gameState;

int revealedTilesCount;
int minesPresentCount;

// Tile constructor
Tile::Tile() : x(0), y(0), isMine(false), isRevealed(false), isFlagged(false), nearbyMineCount(-1) {}

Game::Game() : currentDifficulty(easy), firstClickMade(false) {}

void Game::GamePlaySound(int sound)
{
    if (isSoundEnabled == true)
    {
        PlaySound(sounds[sound]);
    }
}
//Set Email
void Game::setEmail(string userEmail){
  email=userEmail;
}

// Checks if a given tile index is within bounds
bool Game::IsTileIndexValid(int col, int row)
{
    return col >= 0 && col < currentDifficulty.cols && row >= 0 && row < currentDifficulty.rows;
}

// Recursively reveals neighboring tiles if they have zero adjacent mines
void Game::RevealTilesFrom(int col, int row)
{
    for (int colOffset = -1; colOffset <= 1; colOffset++)
    {
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++)
        {
            if (colOffset == 0 && rowOffset == 0)
                continue;
            if (!IsTileIndexValid(col + colOffset, row + rowOffset))
                continue;
            RevealTile(col + colOffset, row + rowOffset);
        }
    }
}

// Toggles flag on a tile
void Game::FlagTile(int col, int row)
{
    if (grid[col][row].isRevealed)
    {
        return;
    }

    grid[col][row].isFlagged = !grid[col][row].isFlagged;
    GamePlaySound(SOUND_THREE);
    SetSoundVolume(sounds[SOUND_THREE], 1.0f);
}

// Reveals a tile and checks game end conditions
void Game::RevealTile(int col, int row)
{
    if (!firstClickMade)
    {
        PlaceMinesExcluding(col, row); // First click safety
        firstClickMade = true;
    }

    if (grid[col][row].isFlagged || grid[col][row].isRevealed)
        return;

    grid[col][row].isRevealed = true;

    if (grid[col][row].isMine)
    {
        gameState = STATE_LOSE;
        timeGameEnded = (float)GetTime();
        GamePlaySound(SOUND_ONE);
        SetSoundVolume(sounds[SOUND_ONE], 1.0f);
    }
    else
    {
        if (grid[col][row].nearbyMineCount == 0)
        {
            RevealTilesFrom(col, row);
        }

        revealedTilesCount++;

        if (revealedTilesCount >= (GetCols() * GetRows() - minesPresentCount))
        {
            gameState = STATE_WIN;
            timeGameEnded = (float)GetTime();
        }
    }
}

// Counts the number of mines around a given tile
int Game::countNearbyMines(int col, int row)
{
    int count = 0;
    for (int colOffset = -1; colOffset <= 1; colOffset++)
    {
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++)
        {
            if (colOffset == 0 && rowOffset == 0)
                continue;
            if (!IsTileIndexValid(col + colOffset, row + rowOffset))
                continue;
            if (grid[col + colOffset][row + rowOffset].isMine)
                count++;
        }
    }
    return count;
}

// Initializes all tiles and resets states
void Game::ResetTiles()
{
    grid = vector<vector<Tile>>(GetCols(), vector<Tile>(GetRows()));

    for (int i = 0; i < GetCols(); i++)
    {
        for (int j = 0; j < GetRows(); j++)
        {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].isMine = false;
            grid[i][j].isRevealed = false;
            grid[i][j].isFlagged = false;
            grid[i][j].nearbyMineCount = -1;
        }
    }
}

void Game::PlaceMinesExcluding(int safeCol, int safeRow)
{
    int minesToPlace = currentDifficulty.mines;
    minesPresentCount = currentDifficulty.mines;

    while (minesToPlace > 0)
    {
        int col = GetRandomValue(0, GetCols() - 1);
        int row = GetRandomValue(0, GetRows() - 1);

        if ((col == safeCol && row == safeRow) || grid[col][row].isMine)
            continue;

        grid[col][row].isMine = true;
        minesToPlace--;
    }

    for (int i = 0; i < GetCols(); i++)
    {
        for (int j = 0; j < GetRows(); j++)
        {
            if (!grid[i][j].isMine)
            {
                grid[i][j].nearbyMineCount = countNearbyMines(i, j);
            }
        }
    }
}

void Game::RenderTile(Tile tile)
{
    if (tile.isRevealed)
    {
        if (tile.isMine)
        {
            DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), GetTileHeight(), RED);
            Rectangle source = {0, 0, (float)textures[TEXTURE_BOMB].width, (float)textures[TEXTURE_BOMB].height};
            Rectangle dest = {(float)(tile.x * GetTileWidth()), (float)(tile.y * GetTileHeight()), (float)GetTileWidth(), (float)GetTileHeight()};
            Vector2 origin = {0, 0};
            DrawTexturePro(textures[TEXTURE_BOMB], source, dest, origin, 0.0f, WHITE);
        }
        else
        {
            int x = tile.x * GetTileWidth();
            int y = tile.y * GetTileHeight();
            int w = GetTileWidth();
            int h = GetTileHeight();

            Color base = {220, 220, 220, 255};
            Color highlight = {255, 255, 255, 255};
            Color shadow = {180, 180, 180, 255};

            DrawRectangle(x, y, w, h, base);
            DrawLine(x, y, x + w - 1, y, highlight);
            DrawLine(x, y, x, y + h - 1, highlight);
            DrawLine(x, y + h - 1, x + w - 1, y + h - 1, shadow);
            DrawLine(x + w - 1, y, x + w - 1, y + h - 1, shadow);
            DrawRectangleLines(x + 2, y + 2, w - 4, h - 4, Fade(DARKGRAY, 0.15f));

            if (tile.nearbyMineCount > 0)
            {
                Color textColor;
                switch (tile.nearbyMineCount)
                {
                case 1:
                    textColor = BLUE;
                    break;
                case 2:
                    textColor = GREEN;
                    break;
                case 3:
                    textColor = RED;
                    break;
                case 4:
                    textColor = PURPLE;
                    break;
                case 5:
                    textColor = BLACK;
                    break;
                default:
                    textColor = DARKGRAY;
                    break;
                }

                int fontSize = GetTileHeight() - 8;
                std::string numText = TextFormat("%d", tile.nearbyMineCount);
                int textWidth = MeasureText(numText.c_str(), fontSize);
                int textX = x + (w - textWidth) / 2;
                int textY = y + (h - fontSize) / 2;

                DrawText(numText.c_str(), textX, textY, fontSize, textColor);
            }
        }
    }
    else
    {
        Color win7Blue = {28, 99, 204, 255};
        Color highlight = {101, 164, 223, 255};
        Color shadow = {10, 50, 110, 255};

        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), GetTileHeight(), win7Blue);
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), 2, highlight);
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), 2, GetTileHeight(), highlight);
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight() + GetTileHeight() - 2, GetTileWidth(), 2, shadow);
        DrawRectangle(tile.x * GetTileWidth() + GetTileWidth() - 2, tile.y * GetTileHeight(), 2, GetTileHeight(), shadow);

        if (tile.isFlagged)
        {
            Rectangle source = {0, 0, (float)textures[TEXTURE_FLAG].width, (float)textures[TEXTURE_FLAG].height};
            Rectangle dest = {(float)(tile.x * GetTileWidth()), (float)(tile.y * GetTileHeight()), (float)(GetTileWidth()), (float)(GetTileHeight())};
            Vector2 origin = {0, 0};
            DrawTexturePro(textures[TEXTURE_FLAG], source, dest, origin, 0.0f, WHITE);
        }
    }

    DrawRectangleLines(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), GetTileHeight(), BLACK);
}

void Game::RenderTiles()
{
    for (int i = 0; i < currentDifficulty.cols; i++)
    {
        for (int j = 0; j < currentDifficulty.rows; j++)
        {
            RenderTile(grid[i][j]);
        }
    }
}

void Game::GameReset()
{
    ResetTiles();
    firstClickMade = false;
    gameState = STATE_PLAYING;
    revealedTilesCount = 0;
    timeGameStarted = GetTime();
    timeGameEnded = 0.0f;
}

void Game::GameStartup()
{
    ui.Init();
    gameFont = LoadFontEx("D:/Anonymous/Developer/MyRaylib/MineSweeper/hello_raylib/assets/fonts/Poppins/Poppins/Poppins-SemiBold.ttf", 48, 0, 250);
    InitAudioDevice();
    Image image1 = LoadImage("../assets/images/flag.png");
    textures[TEXTURE_FLAG] = LoadTextureFromImage(image1);
    UnloadImage(image1);

    Image image2 = LoadImage("../assets/images/bomb.png");
    textures[TEXTURE_BOMB] = LoadTextureFromImage(image2);
    UnloadImage(image2);

    sounds[SOUND_ONE] = LoadSound("../assets/images/bomb.wav");
    sounds[SOUND_TWO] = LoadSound("../assets/images/start.wav");
    sounds[SOUND_THREE] = LoadSound("../assets/images/flag.wav");

    music[MUSIC_ONE] = LoadMusicStream("../assets/images/music.mp3");

    PlayMusicStream(music[MUSIC_ONE]);
    SetMusicVolume(music[MUSIC_ONE], 0.1f);

    gameState = STATE_LOADING;
    loadingTime = 0.0f;

}

void Game::GameUpdate()
{
    UpdateMusicStream(music[MUSIC_ONE]);

    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    switch (gameState)
    {
    case STATE_LOADING:
    {
        loadingTime += GetFrameTime();
        if (loadingTime >= loadingDuration)
        {
            gameState = STATE_HOME_MENU;
        }
        break;
    }
    case STATE_HOME_MENU:
    {
        int clicked = ui.UpdateHome(mousePos, mousePressed);
        if (clicked == 0) // New Game
            gameState = STATE_LEVEL_SELECTION;
        else if (clicked == 1) // Options
            gameState = STATE_OPTIONS_MENU;
        else if (clicked == 2) // Leaderboard
            gameState = STATE_LEADERBOARD;
        else if (clicked == 3) // How to play
            gameState = STATE_HOW_TO_PLAY;
        else if (clicked == 4) // Exit
            CloseWindow();
        break;
    }
    case STATE_OPTIONS_MENU:
    {
        int clicked = ui.UpdateOptions(mousePos, mousePressed);
        if (clicked == 0) // Toggle sound
            isSoundEnabled = !isSoundEnabled;
        else if (clicked == 1) // Toggle music
        {
            isMusicEnabled = !isMusicEnabled;
            if (isMusicEnabled)
                PlayMusicStream(music[MUSIC_ONE]);
            else
                StopMusicStream(music[MUSIC_ONE]);
        }
        else if (clicked == 2) // Back
            gameState = STATE_HOME_MENU;
        break;
    }
    case STATE_LEVEL_SELECTION:
    {
        int clicked = ui.UpdateLevelSelection(mousePos, mousePressed);
        if (clicked == 0)
        {
            currentDifficulty = easy;
            GameReset();
        }
        else if (clicked == 1)
        {
            currentDifficulty = medium;
            GameReset();
        }
        else if (clicked == 2)
        {
            currentDifficulty = hard;
            GameReset();
        }
        break;
    }
    case STATE_HOW_TO_PLAY:
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            gameState = STATE_HOME_MENU;
        }
        break;
    }

    case STATE_PLAYING:
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            int col = (int)(mousePos.x / GetTileWidth());
            int row = (int)(mousePos.y / GetTileHeight());
            if (IsTileIndexValid(col, row))
            {
                RevealTile(col, row);
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mousePos = GetMousePosition();
            int col = (int)(mousePos.x / GetTileWidth());
            int row = (int)(mousePos.y / GetTileHeight());
            if (IsTileIndexValid(col, row))
                FlagTile(col, row);
        }
        break;
    case STATE_LOSE:
        if (IsKeyPressed(KEY_ENTER))
        {
            GamePlaySound(SOUND_TWO);
            gameState = STATE_HOME_MENU;
        }
        break;
    case STATE_WIN:
        if (IsKeyPressed(KEY_ENTER))
        {
            GamePlaySound(SOUND_TWO);
            gameState = STATE_HOME_MENU;
        }
        break;
    }
}

void Game::GameShutdown()
{
    ui.Unload();
    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        UnloadTexture(textures[i]);
    }

    for (int i = 0; i < MAX_SOUNDS; i++)
    {
        UnloadSound(sounds[i]);
    }

    StopMusicStream(music[MUSIC_ONE]);
    UnloadMusicStream(music[MUSIC_ONE]);
    UnloadFont(gameFont);

    CloseAudioDevice();
}

void Game::GameRender()
{
    switch (gameState)
    {
    case STATE_LOADING:
    {
        // Calculate alpha for fade out (from 1 to 0)
        float alpha = 1.0f - (loadingTime / loadingDuration);
        if (alpha < 0)
            alpha = 0;

        ClearBackground(BLACK);

        // Draw bomb texture centered with alpha
        int texW = textures[TEXTURE_BOMB].width;
        int texH = textures[TEXTURE_BOMB].height;
        float scale = 2.0f; // scale up if needed

        Vector2 pos = {(screen_width - texW * scale) / 2.0f, (screen_height - texH * scale) / 2.0f};

        DrawTextureEx(textures[TEXTURE_BOMB], pos, 0.0f, scale, Fade(WHITE, alpha));

        break;
    }
    case STATE_HOME_MENU: // or STATE_HOME_MENU
        ui.DrawHome();
        break;
    case STATE_OPTIONS_MENU:
        ui.DrawOptions(isSoundEnabled, isMusicEnabled);
        break;
    case STATE_LEVEL_SELECTION:
        ui.DrawLevelSelection();
        updated=false;
        break;
    case STATE_PLAYING:
    {
        RenderTiles();
        break;
    }
    case STATE_HOW_TO_PLAY:
    {
        ui.DrawHowToPlay();
        break;
    }

    case STATE_LOSE:
    {
        RenderTiles(); // Optionally draw the grid behind the overlay
        float elapsed = timeGameEnded - timeGameStarted;
        DrawEndScreen(false, elapsed);
        break;
    }
    case STATE_WIN:
    {
        RenderTiles();
        float elapsed = timeGameEnded - timeGameStarted;

        DrawEndScreen(true, elapsed);
        break;
    }
    }
}
void Game::DrawEndScreen(bool isWin, float timePlayed)
{
    // Dim background overlay
    DrawRectangle(0, 0, screen_width, screen_height, Fade(BLACK, 0.85f));

    // Glow colors and font size
    const char *label = isWin ? "YOU WIN!" : "GAME OVER";
    Color glowColor = isWin ? SKYBLUE : RED;
    int fontSize = 72;

    Vector2 textSize = MeasureTextEx(gameFont, label, fontSize, 2);
    Vector2 textPos = {(screen_width - textSize.x) / 2, screen_height / 3.0f};

    // Glow effect: multiple draws with fading alpha
    for (int i = 4; i >= 1; i--)
    {
        float alpha = 0.1f * i;
        DrawTextEx(gameFont, label, textPos, fontSize, 2, Fade(glowColor, alpha));
    }
    DrawTextEx(gameFont, label, textPos, fontSize, 2, WHITE);

    // Draw time played
    int minutes = (int)(timePlayed / 60);
    int seconds = (int)(timePlayed) % 60;
    char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "Time Played: %02d:%02d", minutes, seconds);
    //Backend Integration To Update Score
     if(!updated){
       if(isWin){
           Score add(email,timePlayed,1,0);
           result=add.sendRequest();
           updated=true;
      cout<<result;


    }else{
      Score add(email,0,0,1);
           result=add.sendRequest();
                updated=true;
      cout<<result;
    }

     }


    Vector2 timeSize = MeasureTextEx(gameFont, timeStr, 36, 1);
    Vector2 timePos = {(screen_width - timeSize.x) / 2, textPos.y + fontSize + 40};

    // Draw circle as clock icon
    DrawCircle(timePos.x - 40, timePos.y + 18, 16, glowColor);
    DrawTextEx(gameFont, timeStr, timePos, 36, 1, WHITE);

    // Blinking "Press ENTER" prompt
    static float blinkTimer = 0.0f;
    blinkTimer += GetFrameTime();
    bool showPrompt = ((int)(blinkTimer * 2) % 2) == 0;

    const char *prompt = "Press ENTER to return to Main Menu";
    Vector2 promptSize = MeasureTextEx(gameFont, prompt, 28, 1);
    Vector2 promptPos = {(screen_width - promptSize.x) / 2, screen_height * 0.75f};

    if (showPrompt){

       DrawTextEx(gameFont, prompt, promptPos, 28, 1, Fade(WHITE, 0.8f));

    }

}
int Game::GetCols()
{
    return currentDifficulty.cols;
}

int Game::GetRows()
{
    return currentDifficulty.rows;
}

int Game::GetTileWidth()
{
    return screen_width / currentDifficulty.cols;
}

int Game::GetTileHeight()
{
    return screen_height / currentDifficulty.rows;
}
