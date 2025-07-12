#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <raylib.h>

void RenderLeaderboardScreen(Font &font, int screenWidth, int screenHeight);

struct LeaderboardEntry {
    std::string name;
    float timeSeconds;
};

extern LeaderboardEntry top3[3];
extern LeaderboardEntry others[2];

std::string FormatTime(float seconds);
void DrawBackgroundGradient(int width, int height);
void DrawGlowingTitle(Font &font, const char *text, int screenWidth);
void DrawPodiumBlock(int rank, const LeaderboardEntry &entry, Rectangle rect, Font &font);
void DrawRankedList(const LeaderboardEntry entries[], int count, Font &font, int screenWidth, int startY);

#endif
