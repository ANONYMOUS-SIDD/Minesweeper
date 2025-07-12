#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <raylib.h>
#include <algorithm>


// Struct to hold leaderboard display info
struct LeaderboardEntry {
    std::string name;
    float timeSeconds;
};

// Struct to hold player data from backend
struct PlayerData {
    std::string user_name;
    std::string email;
    int bestTime;
    int won;
    int lose;
};

// Responsible for fetching player data from backend
class LeaderBoard {
public:
    static std::vector<PlayerData> fetchTopPlayers(const std::string& url);
};

// Renders the complete leaderboard UI
void RenderLeaderboardScreen(Font &font, int screenWidth, int screenHeight);

// Utility rendering and helper functions
std::string FormatTime(float seconds);
void DrawBackgroundGradient(int width, int height);
void DrawGlowingTitle(Font &font, const char *text, int screenWidth);
void DrawPodiumBlock(int rank, const LeaderboardEntry &entry, Rectangle rect, Font &font);
void DrawRankedList(const std::vector<LeaderboardEntry>& entries, Font &font, int screenWidth, int startY);

#endif // LEADERBOARD_H
