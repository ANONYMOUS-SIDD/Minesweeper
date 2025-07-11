#ifndef SWEEP_H
#define SWEEP_H

#include <raylib.h>
#include <vector>
using namespace std;

#define MAX_TEXTURES 2
#define MAX_SOUNDS 3
#define MAX_MUSIC 1

// Forward declaration of UIManager to fix the error
struct UIManager;

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

struct Difficulty
{
    int cols;
    int rows;
    int mines;
};

extern Difficulty easy;
extern Difficulty medium;
extern Difficulty hard;

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
    Tile();
};

// Texture index definitions
typedef enum
{
    TEXTURE_FLAG = 0,
    TEXTURE_BOMB
} texture_asset;

// Texture array for assets
extern Texture2D textures[MAX_TEXTURES];

typedef enum
{
    SOUND_ONE = 0,
    SOUND_TWO,
    SOUND_THREE,
} sound_asset;

extern Sound sounds[MAX_SOUNDS];

typedef enum
{
    MUSIC_ONE = 0
} music_asset;

extern Music music[MAX_MUSIC];

// Game state definitions
typedef enum
{
    STATE_LOADING = -1,
    STATE_HOME_MENU = 0,
    STATE_OPTIONS_MENU = 1,
    STATE_LEVEL_SELECTION = 2,
    STATE_PLAYING = 3,
    STATE_HOW_TO_PLAY = 4,
    STATE_LEADERBOARD = 5,
    STATE_LOSE = 6,
    STATE_WIN = 7,
} game_states;

extern game_states gameState;

extern int revealedTilesCount;
extern int minesPresentCount;

// Game class encapsulating game logic
class Game
{
private:
    // Grid of tiles
    vector<vector<Tile>> grid;
    Difficulty currentDifficulty;
    bool firstClickMade;

public:
    Game();

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
    void DrawEndScreen(bool, float);    // Draw end screen with win/lose message

    // Global variables for grid dimensions
    int GetCols();
    int GetRows();
    int GetTileWidth();
    int GetTileHeight();
};
#endif // SWEEP_H