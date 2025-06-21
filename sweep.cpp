#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
using namespace std;

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
    STATE_MAIN_MENU = 0,
    STATE_OPTIONS_MENU,
    STATE_PLAYING,
    STATE_LOSE,
    STATE_WIN,
    STATE_LEVEL_SELECTION
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

// Initializes all tiles and randomly places mines
void Game::ResetTiles()
{
    // Resize grid dynamically based on current difficulty
    grid = vector<vector<Tile>>(GetCols(), vector<Tile>(GetRows()));

    // Initialize each tile's x and y position
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

        // Don’t place mine at (safeCol, safeRow)
        if ((col == safeCol && row == safeRow) || grid[col][row].isMine)
            continue;

        grid[col][row].isMine = true;
        minesToPlace--;
    }

    // Now calculate nearby mine counts
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

// Renders an individual tile based on its state
void Game::RenderTile(Tile tile)
{
    if (tile.isRevealed)
    {
        if (tile.isMine)
        {
            // First draws the red background
            DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), GetTileHeight(), RED);
            // Then draws the bomb texture
            Rectangle source = {0, 0, (float)textures[TEXTURE_BOMB].width, (float)textures[TEXTURE_BOMB].height};
            Rectangle dest = {
                (float)(tile.x * GetTileWidth()),
                (float)(tile.y * GetTileHeight()),
                (float)GetTileWidth(),
                (float)GetTileHeight()};
            Vector2 origin = {0, 0};
            DrawTexturePro(textures[TEXTURE_BOMB], source, dest, origin, 0.0f, WHITE);
        }
        else
        {
            int x = tile.x * GetTileWidth();
            int y = tile.y * GetTileHeight();
            int w = GetTileWidth();
            int h = GetTileHeight();

            // Base color with bevel
            Color base = {220, 220, 220, 255};      // Soft light gray
            Color highlight = {255, 255, 255, 255}; // Top-left bevel
            Color shadow = {180, 180, 180, 255};    // Bottom-right bevel

            // Fill tile background
            DrawRectangle(x, y, w, h, base);

            // Draw bevel edges
            DrawLine(x, y, x + w - 1, y, highlight);              // top
            DrawLine(x, y, x, y + h - 1, highlight);              // left
            DrawLine(x, y + h - 1, x + w - 1, y + h - 1, shadow); // bottom
            DrawLine(x + w - 1, y, x + w - 1, y + h - 1, shadow); // right

            // Optional inner inset to simulate depth
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
                    textColor = DARKGRAY; // Fallback for 6,7,8
                    break;
                }
                DrawText(TextFormat("%d", tile.nearbyMineCount), tile.x * GetTileWidth() + 13, tile.y * GetTileHeight() + 4, GetTileHeight() - 8, textColor);
            }
        }
    }
    else
    {
        Color win7Blue = {28, 99, 204, 255};    // Main tile color
        Color highlight = {101, 164, 223, 255}; // Lighter blue for highlight (like a sheen)
        Color shadow = {10, 50, 110, 255};      // Darker blue for shadow edges

        // Draw base tile
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), GetTileHeight(), win7Blue);

        // Draw top highlight line (1 pixel height)
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), 2, highlight);

        // Draw left highlight line (1 pixel width)
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight(), 2, GetTileHeight(), highlight);

        // Draw bottom shadow line (1 pixel height)
        DrawRectangle(tile.x * GetTileWidth(), tile.y * GetTileHeight() + GetTileHeight() - 2, GetTileWidth(), 2, shadow);

        // Draw right shadow line (1 pixel width)
        DrawRectangle(tile.x * GetTileWidth() + GetTileWidth() - 2, tile.x * GetTileHeight(), 2, GetTileHeight(), shadow);
        if (tile.isFlagged)
        {
            Rectangle source = {0, 0, (float)textures[TEXTURE_FLAG].width, (float)textures[TEXTURE_FLAG].height};
            Rectangle dest = {(float)(tile.x * GetTileWidth()), (float)(tile.y * GetTileHeight()), (float)(GetTileWidth()), (float)(GetTileHeight())};
            Vector2 origin = {0, 0};
            DrawTexturePro(textures[TEXTURE_FLAG], source, dest, origin, 0.0f, WHITE);
        }
    }

    // Draw grid lines
    DrawRectangleLines(tile.x * GetTileWidth(), tile.y * GetTileHeight(), GetTileWidth(), GetTileHeight(), BLACK);
}

// Renders the entire grid of tiles
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

// Resets the game state and tiles
void Game::GameReset()
{
    ResetTiles();
    firstClickMade = false; // Important because we need to place mines after the first click
    gameState = STATE_PLAYING;
    revealedTilesCount = 0;
    timeGameStarted = (float)GetTime();
}

// Loads textures and sets initial game state
void Game::GameStartup()
{
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

    gameState = STATE_MAIN_MENU;
}

// Handles input and updates game logic based on current state
void Game::GameUpdate()
{

    UpdateMusicStream(music[MUSIC_ONE]);

    switch (gameState)
    {
    case STATE_MAIN_MENU:
        if (IsKeyPressed(KEY_N))
        {
            GamePlaySound(SOUND_TWO);
            SetSoundVolume(sounds[SOUND_TWO], 1.0f);
            gameState = STATE_LEVEL_SELECTION;
        }
        else if (IsKeyPressed(KEY_O))
        {
            gameState = STATE_OPTIONS_MENU;
            GamePlaySound(SOUND_TWO);
        }
        break;
    case STATE_OPTIONS_MENU:
        if (IsKeyPressed(KEY_ENTER))
        {
            gameState = STATE_MAIN_MENU;
            GamePlaySound(SOUND_TWO);
        }
        if (IsKeyPressed(KEY_S))
        {
            isSoundEnabled = !isSoundEnabled;
            PlaySound(sounds[SOUND_TWO]);
        }
        if (IsKeyPressed(KEY_M))
        {
            isMusicEnabled = !isMusicEnabled;
            PlaySound(sounds[SOUND_TWO]);
            if (isMusicEnabled)
            {
                StopMusicStream(music[MUSIC_ONE]);
                PlayMusicStream(music[MUSIC_ONE]);
            }
            else
            {
                StopMusicStream(music[MUSIC_ONE]);
            }
        }
        break;
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
            SetSoundVolume(sounds[SOUND_ONE], 1.0f);
            gameState = STATE_MAIN_MENU;
        }
        break;
    case STATE_WIN:
        if (IsKeyPressed(KEY_ENTER))
        {
            GamePlaySound(SOUND_TWO);
            SetSoundVolume(sounds[SOUND_ONE], 1.0f);
            gameState = STATE_MAIN_MENU;
        }
        break;
    case STATE_LEVEL_SELECTION:
        if (IsKeyPressed(KEY_ONE))
        {
            currentDifficulty = easy;
            GameReset();
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            currentDifficulty = medium;
            GameReset();
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            currentDifficulty = hard;
            GameReset();
        }
        break;
    }
}

// Frees all textures and shuts down audio
void Game::GameShutdown()
{
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

    CloseAudioDevice();
}

// Handles rendering for all game states
void Game::GameRender()
{
    int seconds = 0;
    switch (gameState)
    {
    case STATE_MAIN_MENU:
        DrawRectangle(0, 0, screen_width, screen_height, DARKBLUE);
        DrawText("MINESWEEPER", 20, 20, 40, WHITE);
        DrawText("[N]ew Game", 120, 220, 20, WHITE);
        DrawText("[O]ptions", 120, 250, 20, WHITE);
        DrawText("ESC to QUIT", 120, 280, 20, WHITE);
        DrawText("PROGRAMMED BY PRASIDDHA", 120, 500, 20, WHITE);
        break;
    case STATE_OPTIONS_MENU:
        DrawRectangle(0, 0, screen_width, screen_height, DARKBLUE);
        DrawText("Minesweeper::Options", 20, 20, 40, WHITE);
        DrawText("Press Enter For Main Menu", 150, 400, 40, RED);
        DrawText("[S]ound", 120, 220, 20, WHITE);
        if (isSoundEnabled)
        {
            DrawText("ON", 280, 220, 20, YELLOW);
            DrawText("/", 310, 220, 20, WHITE);
            DrawText("OFF", 350, 220, 20, WHITE);
        }
        else
        {
            DrawText("ON", 280, 220, 20, WHITE);
            DrawText("/", 310, 220, 20, WHITE);
            DrawText("OFF", 350, 220, 20, YELLOW);
        }
        DrawText("[M]usic", 120, 250, 20, WHITE);
        if (isMusicEnabled)
        {
            DrawText("ON", 280, 250, 20, YELLOW);
            DrawText("/", 310, 250, 20, WHITE);
            DrawText("OFF", 350, 250, 20, WHITE);
        }
        else
        {
            DrawText("ON", 280, 250, 20, WHITE);
            DrawText("/", 310, 250, 20, WHITE);
            DrawText("OFF", 350, 250, 20, YELLOW);
        }
        break;
    case STATE_PLAYING:
        RenderTiles();
        break;
    case STATE_LOSE:
        RenderTiles();
        DrawRectangle(0, 0, screen_width, screen_height, Fade(WHITE, 0.8f));
        DrawText(labelGameLose, screen_width / 2 - MeasureText(labelGameLose, 60) / 2, screen_height / 2 - 10, 60, DARKGRAY);
        DrawText(labelEnter, screen_width / 2 - MeasureText(labelEnter, 60) / 2, (int)(screen_height * 0.75f) - 10, 34, DARKGRAY);
        seconds = (int)(timeGameEnded - timeGameStarted) % 60;
        DrawText(TextFormat("TIME PLAYED: %d s", seconds), 20, 40, 34, DARKGRAY);
        break;
    case STATE_WIN:
        RenderTiles();
        DrawRectangle(0, 0, screen_width, screen_height, Fade(WHITE, 0.8f));
        DrawText(gameState == STATE_LOSE ? labelGameLose : labelGameWin, screen_width / 2 - MeasureText(gameState == STATE_LOSE ? labelGameLose : labelGameWin, 60) / 2, screen_height / 2 - 10, 60, DARKGRAY);
        DrawText(labelEnter, screen_width / 2 - MeasureText(labelEnter, 60) / 2, (int)(screen_height * 0.75f) - 10, 34, DARKGRAY);
        seconds = (int)(timeGameEnded - timeGameStarted) % 60;
        DrawText(TextFormat("TIME PLAYED: %d s", seconds), 20, 40, 34, DARKGRAY);
        break;
    case STATE_LEVEL_SELECTION:
        DrawRectangle(0, 0, screen_width, screen_height, DARKBLUE);
        DrawText("Select Difficulty", 250, 100, 40, WHITE);
        DrawText("[1] LEVEL 1 - EASY (12x12)", 200, 200, 30, LIGHTGRAY);
        DrawText("[2] LEVEL 2 - MEDIUM (15x15)", 200, 250, 30, LIGHTGRAY);
        DrawText("[3] LEVEL 3 - HARD (20x20)", 200, 300, 30, LIGHTGRAY);
        break;
    }
}

// Entry point of the application
int main()
{
    InitWindow(screen_width, screen_height, "Minesweeper game");
    SetTargetFPS(60);
    Game game;
    game.GameStartup();
    while (!WindowShouldClose())
    {
        game.GameUpdate();
        BeginDrawing();
        ClearBackground(DARKBLUE);
        game.GameRender();
        EndDrawing();
    }
    game.GameShutdown();
    CloseWindow();
    return 0;
}