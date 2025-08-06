#include "leaderboard.h"
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

// Format time in MM:SS
std::string FormatTime(float seconds) {
    int mins = (int)(seconds / 60);
    int secs = (int)seconds % 60;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", mins, secs);
    return std::string(buffer);
}

// Gradient background
void DrawBackgroundGradient(int width, int height) {
    for (int y = 0; y < height; y += 4) {
        float t = (float)y / height;
        Color c = ColorLerp(Color{15, 15, 25, 255}, Color{5, 5, 10, 255}, t);
        DrawRectangle(0, y, width, 4, c);
    }
}

// Glowing title text
void DrawGlowingTitle(Font &font, const char *text, int screenWidth) {
    int fontSize = 72;
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 2);
    Vector2 pos = {(screenWidth - textSize.x) / 2, 40};

    // Subtle glow effect
    for (int i = 4; i >= 1; i--) {
        float alpha = 0.1f * i;
        float offsetX = (float)GetRandomValue(-10, 10) / 10.0f;
        float offsetY = (float)GetRandomValue(-10, 10) / 10.0f;
        DrawTextEx(font, text, {pos.x + offsetX, pos.y + offsetY}, fontSize, 2, Fade(Color{100, 150, 255, 255}, alpha));
    }
    DrawTextEx(font, text, pos, fontSize, 2, WHITE);
}

// A versatile function to draw a single leaderboard card
void DrawLeaderboardCard(int rank, const LeaderboardEntry& entry, Rectangle rect, Font& font, bool isHovered, bool isPodium) {
    Color rankBgColor = Fade(Color{30, 30, 50, 255}, 0.8f);
    Color nameColor = WHITE;
    Color timeColor = Color{147, 197, 253, 255};
    Color rankNumberColor = Fade(Color{107, 114, 128, 255}, 0.7f);
    Color borderColor = Color{40, 40, 60, 255};

    if (isPodium) {
        // Poduim players get special colors and sizes
        switch (rank) {
            case 1:
                borderColor = Color{255, 215, 0, 255}; // Gold
                rankNumberColor = Color{255, 215, 0, 255};
                break;
            case 2:
                borderColor = Color{192, 192, 192, 255}; // Silver
                rankNumberColor = Color{192, 192, 192, 255};
                break;
            case 3:
                borderColor = Color{205, 127, 50, 255}; // Bronze
                rankNumberColor = Color{205, 127, 50, 255};
                break;
        }
    } else if (isHovered) {
        rankBgColor = Fade(Color{50, 60, 80, 255}, 0.9f);
        borderColor = Color{59, 130, 246, 255};
    }

    // Main background of the card
    DrawRectangleRounded(rect, 0.2f, 8, rankBgColor);

    // Border line
    DrawRectangleRoundedLines(rect, 0.2f, 8, borderColor);

    // Rank number text
    std::string rankStr = std::to_string(rank);
    int rankFontSize = isPodium ? (rank == 1 ? 52 : 40) : 28;
    Vector2 rankSize = MeasureTextEx(font, rankStr.c_str(), rankFontSize, 2);
    Vector2 rankPos;
    if (isPodium) {
        rankPos = {rect.x + (rect.width - rankSize.x) / 2, rect.y + 20};
    } else {
        rankPos = {rect.x + 20, rect.y + (rect.height - rankSize.y) / 2};
    }
    DrawTextEx(font, rankStr.c_str(), rankPos, rankFontSize, 2, rankNumberColor);

    // Name text
    int nameFontSize = isPodium ? 28 : 24;
    Vector2 nameSize = MeasureTextEx(font, entry.name.c_str(), nameFontSize, 1);
    Vector2 namePos;
    if (isPodium) {
        namePos = {rect.x + (rect.width - nameSize.x) / 2, rect.y + rect.height - 70};
    } else {
        namePos = {rect.x + 70, rect.y + (rect.height - nameSize.y) / 2};
    }
    DrawTextEx(font, entry.name.c_str(), namePos, nameFontSize, 1, nameColor);

    // Time text
    std::string timeStr = FormatTime(entry.timeSeconds);
    int timeFontSize = isPodium ? 24 : 24;
    Vector2 timeSize = MeasureTextEx(font, timeStr.c_str(), timeFontSize, 1);
    Vector2 timePos;
    if (isPodium) {
        timePos = {rect.x + (rect.width - timeSize.x) / 2, rect.y + rect.height - 35};
    } else {
        timePos = {rect.x + rect.width - timeSize.x - 20, rect.y + (rect.height - timeSize.y) / 2};
    }
    DrawTextEx(font, timeStr.c_str(), timePos, timeFontSize, 1, timeColor);
}

// Main render function
void RenderLeaderboardScreen(Font &font, int screenWidth, int screenHeight) {
    DrawBackgroundGradient(screenWidth, screenHeight);
    DrawGlowingTitle(font, "LEADERBOARD", screenWidth);

    // Fetch and sort players (logic kept as-is)
    std::vector<PlayerData> players = LeaderBoard::fetchTopPlayers("http://localhost:18080/playersInfo");
    std::sort(players.begin(), players.end(), [](const PlayerData& a, const PlayerData& b) {
        return a.bestTime < b.bestTime;
    });

    // Convert to LeaderboardEntry
    std::vector<LeaderboardEntry> entries;
    for (const auto& player : players) {
        entries.push_back({player.user_name, (float)player.bestTime});
    }

    // Fallback if empty
    if (entries.size() < 5) {
        entries = {
            {"Siddhant Timalsina", 58 * 60 + 12},
            {"Prasiddha Timalsina", 65 * 60 + 33},
            {"Arip Sunar", 70 * 60 + 2},
            {"Sushant Timalsina", 75 * 60 + 42},
            {"Sapana Sapkota", 88 * 60 + 19}
        };
    }

    // --- Draw Top 3 Display ---
    int podiumStartY = 160;
    float podiumWidth = screenWidth * 0.2f;
    float podiumHeight = screenHeight * 0.25f;
    float centerX = screenWidth / 2.0f;
    float padding = 30.0f;

    // Rectangle for top 1
    Rectangle rect1 = {centerX - podiumWidth / 2, (float)podiumStartY, podiumWidth, podiumHeight};
    // Rectangle for top 2
    Rectangle rect2 = {rect1.x - podiumWidth - padding, rect1.y + podiumHeight * 0.15f, podiumWidth, podiumHeight * 0.85f};
    // Rectangle for top 3
    Rectangle rect3 = {rect1.x + podiumWidth + padding, rect1.y + podiumHeight * 0.15f, podiumWidth, podiumHeight * 0.85f};

    if (entries.size() >= 3) {
        // Draw order: 2, 3, then 1 for correct overlap
        DrawLeaderboardCard(2, entries[1], rect2, font, false, true);
        DrawLeaderboardCard(3, entries[2], rect3, font, false, true);
        DrawLeaderboardCard(1, entries[0], rect1, font, false, true);
    }

    // --- Draw Top 5 List at the bottom ---
    int listStartY = podiumStartY + podiumHeight + 80;
    int itemHeight = 60;
    int itemPadding = 15;
    int listWidth = (int)(screenWidth * 0.7f);
    int listStartX = (screenWidth - listWidth) / 2;
    Vector2 mousePos = GetMousePosition();

    // Loop for top 5 players
    int count = std::min((int)entries.size(), 5);
    for (int i = 0; i < count; i++) {
        Rectangle itemRect = {(float)listStartX, (float)(listStartY + i * (itemHeight + itemPadding)), (float)listWidth, (float)itemHeight};
        bool hovered = CheckCollisionPointRec(mousePos, itemRect);
        DrawLeaderboardCard(i + 1, entries[i], itemRect, font, hovered, false);
    }
}
