#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include "home_screen.cpp"
using namespace std;

// This is calling strcture variable from home_screen.cpp
UIManager ui;

Font gameFont;

// Constants defining the grid size and screen resolution
const int screen_width = 900;
const int screen_height = 900;

// Labels for different game states
const char *labelGameWin = "You Win";
const char *labelGameLose = "Game Over";
const char *labelEnter = "Press ENTER for Main Menu";

// Time tracking variables
float timeGameEnded;
float timeGameStarted;

// Sound Variables
bool isSoundEnabled = true;
bool isMusicEnabled = true;

struct Difficulty
{
    int cols;
    int rows;
    int mines;
};

Difficulty easy = {12, 12, (int)(12 * 12 * 0.1f)};    // 10% mines approx
Difficulty medium = {15, 15, (int)(15 * 15 * 0.15f)}; // ~15% mines
Difficulty hard = {20, 20, (int)(20 * 20 * 0.2f)};    // ~20% mines

// Class representing each tile on the grid
class Tile
{
public:
    int x, y;            // Tile position in grid
    bool isMine;         // Is this tile a mine?
    bool isRevealed;     // Has the tile been revealed?
    bool isFlagged;      // Has the tile been flagged?
    int nearbyMineCount; // Number of adjacent mines

    // Constructor to initialize tile with default values
    Tile() : x(0), y(0), isMine(false), isRevealed(false), isFlagged(false), nearbyMineCount(-1) {}
};

// Texture index definitions
#define MAX_TEXTURES 2
typedef enum
{
    TEXTURE_FLAG = 0,
    TEXTURE_BOMB
} texture_asset;

// Texture array for assets
Texture2D textures[MAX_TEXTURES];

#define MAX_SOUNDS 3
typedef enum
{
    SOUND_ONE = 0,
    SOUND_TWO,
    SOUND_THREE,
} sound_asset;

Sound sounds[MAX_SOUNDS];

#define MAX_MUSIC 1
typedef enum
{
    MUSIC_ONE = 0
} music_asset;

Music music[MAX_MUSIC];

// Game state definitions
typedef enum
{
    STATE_HOME_MENU = 0, // New home menu state
    STATE_OPTIONS_MENU,
    STATE_LEVEL_SELECTION,
    STATE_PLAYING,
    STATE_HOW_TO_PLAY,
    STATE_LOSE,
    STATE_WIN,
} game_states;

game_states gameState;

int revealedTilesCount;
int minesPresentCount;

// Game class encapsulating game logic
class Game
{
private:
    // Grid of tiles
    vector<vector<Tile>> grid;
    Difficulty currentDifficulty = easy;
    bool firstClickMade = false;

public:
    void GameStartup();  // Load resources and initialize game
    void GameUpdate();   // Process input and update game state
    void GameShutdown(); // Free resources
    void GameRender();   // Render everything
    void GameReset();    // Reset the game
    void GamePlaySound(int sound);

    void RenderTile(Tile tile);         // Render an individual tile
    void RenderTiles();                 // Render the entire grid
    void ResetTiles();                  // Initialize/reset tiles
    int countNearbyMines(int, int);     // Count mines around a tile
    bool IsTileIndexValid(int, int);    // Check if grid index is valid
    void RevealTile(int, int);          // Reveal a specific tile
    void FlagTile(int, int);            // Toggle flag on a tile
    void RevealTilesFrom(int, int);     // Reveal adjacent tiles
    void PlaceMinesExcluding(int, int); // Place mines excluding a specific tile

    // Global variables for grid dimensions
    int GetCols()
    {
        return currentDifficulty.cols;
    }
    int GetRows()
    {
        return currentDifficulty.rows;
    }
    int GetTileWidth()
    {
        return screen_width / currentDifficulty.cols;
    }
    int GetTileHeight()
    {
        return screen_height / currentDifficulty.rows;
    }
};

void Game::GamePlaySound(int sound)
{
    if (isSoundEnabled == true)
    {
        PlaySound(sounds[sound]);
    }
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
                string numText = TextFormat("%d", tile.nearbyMineCount);
                int fontSize = GetTileHeight() - 8;
                Vector2 textSize = MeasureTextEx(gameFont, numText.c_str(), fontSize, 1);

                float textX = tile.x * GetTileWidth() + (GetTileWidth() - textSize.x) / 2;
                float textY = tile.y * GetTileHeight() + (GetTileHeight() - textSize.y) / 2;

                DrawTextEx(gameFont, numText.c_str(), {textX, textY}, fontSize, 1, textColor);
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
    gameFont = LoadFont("resources/fonts/Merriweather_120pt-Regular.ttf");
    InitAudioDevice();
    Image image1 = LoadImage("minesweeper_assets/flag.png");
    textures[TEXTURE_FLAG] = LoadTextureFromImage(image1);
    UnloadImage(image1);

    Image image2 = LoadImage("minesweeper_assets/bomb.png");
    textures[TEXTURE_BOMB] = LoadTextureFromImage(image2);
    UnloadImage(image2);

    sounds[SOUND_ONE] = LoadSound("minesweeper_assets/bomb.wav");
    sounds[SOUND_TWO] = LoadSound("minesweeper_assets/start.wav");
    sounds[SOUND_THREE] = LoadSound("minesweeper_assets/flag.wav");

    music[MUSIC_ONE] = LoadMusicStream("minesweeper_assets/music.mp3");

    PlayMusicStream(music[MUSIC_ONE]);
    SetMusicVolume(music[MUSIC_ONE], 0.1f);

    gameState = STATE_HOME_MENU; // Start at home menu now
}

void Game::GameUpdate()
{
    UpdateMusicStream(music[MUSIC_ONE]);

    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    switch (gameState)
    {
    case STATE_HOME_MENU:
    {
        int clicked = ui.UpdateHome(mousePos, mousePressed);
        if (clicked == 0) // New Game
            gameState = STATE_LEVEL_SELECTION;
        else if (clicked == 1) // Options
            gameState = STATE_OPTIONS_MENU;
        else if (clicked == 2) // How to Play
            gameState = STATE_HOW_TO_PLAY;
        else if (clicked == 3) // Exit
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
    case STATE_HOME_MENU: // or STATE_HOME_MENU
        ui.DrawHome();
        break;
    case STATE_OPTIONS_MENU:
        ui.DrawOptions(isSoundEnabled, isMusicEnabled);
        break;
    case STATE_LEVEL_SELECTION:
        ui.DrawLevelSelection();
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
        RenderTiles();
        DrawRectangle(0, 0, screen_width, screen_height, Fade(WHITE, 0.8f));
        DrawTextEx(gameFont, labelGameLose, {(float)(screen_width / 2 - MeasureTextEx(gameFont, labelGameLose, 60, 1).x / 2), (float)(screen_height / 2 - 10)}, 60, 1, DARKGRAY);
        DrawTextEx(gameFont, labelEnter, {(float)(screen_width / 2 - MeasureTextEx(gameFont, labelEnter, 34, 1).x / 2), (float)((int)(screen_height * 0.75f) - 10)}, 34, 1, DARKGRAY);
        float elapsed = timeGameEnded - timeGameStarted;
        int minutes = (int)(elapsed / 60);
        int seconds = (int)(elapsed) % 60;
        DrawTextEx(gameFont, TextFormat("TIME PLAYED: %02d:%02d", minutes, seconds), {20.0f, 40.0f}, 34, 1, DARKGRAY);
        break;
    }
    case STATE_WIN:
    {
        RenderTiles();
        DrawRectangle(0, 0, screen_width, screen_height, Fade(WHITE, 0.8f));
        DrawTextEx(gameFont, labelGameWin, {(float)(screen_width / 2 - MeasureTextEx(gameFont, labelGameWin, 60, 1).x / 2), (float)(screen_height / 2 - 10)}, 60, 1, DARKGRAY);
        DrawTextEx(gameFont, labelEnter, {(float)(screen_width / 2 - MeasureTextEx(gameFont, labelEnter, 34, 1).x / 2), (float)((int)(screen_height * 0.75f) - 10)}, 34, 1, DARKGRAY);
        float elapsed = timeGameEnded - timeGameStarted;
        int minutes = (int)(elapsed / 60);
        int seconds = (int)(elapsed) % 60;
        DrawTextEx(gameFont, TextFormat("TIME PLAYED: %02d:%02d", minutes, seconds), {20.0f, 40.0f}, 34, 1, DARKGRAY);
        break;
    }
    }
}

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
