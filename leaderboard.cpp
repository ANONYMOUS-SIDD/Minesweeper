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
        Color c = ColorLerp(Color{30, 30, 40, 255}, Color{15, 15, 20, 255}, t);
        DrawRectangle(0, y, width, 4, c);
    }
}

// Glowing title text
void DrawGlowingTitle(Font &font, const char *text, int screenWidth) {
    int fontSize = 72;
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 2);
    Vector2 pos = {(screenWidth - textSize.x) / 2, 40};

    for (int i = 4; i >= 1; i--) {
        float alpha = 0.1f * i;
        DrawTextEx(font, text, pos, fontSize, 2, Fade(Color{100, 150, 255, 255}, alpha));
    }
    DrawTextEx(font, text, pos, fontSize, 2, WHITE);
}

// Podium blocks for top 3
void DrawPodiumBlock(int rank, const LeaderboardEntry &entry, Rectangle rect, Font &font) {
    Color topColors[3] = {Color{255, 245, 180, 255}, Color{235, 235, 235, 255}, Color{255, 193, 110, 255}};
    Color bottomColors[3] = {Color{255, 215, 60, 255}, Color{180, 180, 180, 255}, Color{205, 127, 50, 255}};
    Color borderColors[3] = {Color{255, 255, 220, 255}, Color{255, 255, 255, 255}, Color{255, 215, 100, 255}};

    DrawRectangleGradientV(rect.x, rect.y, rect.width, rect.height, topColors[rank - 1], bottomColors[rank - 1]);
    DrawRectangle(rect.x, rect.y + rect.height - 3, rect.width, 3, borderColors[rank - 1]);

    int rankFontSize = rank == 1 ? 42 : (rank == 2 ? 34 : 28);
    Vector2 rankSize = MeasureTextEx(font, TextFormat("%d", rank), rankFontSize, 2);
    Vector2 rankPos = {rect.x + (rect.width - rankSize.x) / 2, rect.y + 20};
    DrawTextEx(font, TextFormat("%d", rank), rankPos, rankFontSize, 2, BLACK);

    int nameFontSize = rank == 1 ? 26 : 22;
    Vector2 nameSize = MeasureTextEx(font, entry.name.c_str(), nameFontSize, 1);
    Vector2 namePos = {rect.x + (rect.width - nameSize.x) / 2, rect.y + rect.height - 65};
    DrawTextEx(font, entry.name.c_str(), namePos, nameFontSize, 1, BLACK);

    std::string timeStr = FormatTime(entry.timeSeconds);
    int timeFontSize = rank == 1 ? 22 : 18;
    Vector2 timeSize = MeasureTextEx(font, timeStr.c_str(), timeFontSize, 1);
    Vector2 timePos = {rect.x + (rect.width - timeSize.x) / 2, rect.y + rect.height - 35};
    DrawTextEx(font, timeStr.c_str(), timePos, timeFontSize, 1, Color{230, 230, 180, 255});
}

// Ranked list display
void DrawRankedList(const std::vector<LeaderboardEntry>& entries, Font &font, int screenWidth, int startY) {
    int itemHeight = 50;
    int padding = 12;
    int listWidth = (int)(screenWidth * 0.8f);
    int startX = (screenWidth - listWidth) / 2;

    Vector2 mousePos = GetMousePosition();

    for (size_t i = 0; i < entries.size(); i++) {
        Rectangle itemRect = {(float)startX, (float)(startY + i * (itemHeight + padding)), (float)listWidth, (float)itemHeight};
        bool hovered = CheckCollisionPointRec(mousePos, itemRect);
        Color bgColor = hovered ? Fade(Color{55, 65, 81, 255}, 0.9f) : Fade(Color{55, 65, 81, 255}, 0.7f);
        Color borderColor = hovered ? Color{59, 130, 246, 255} : Color{55, 65, 81, 255};

        DrawRectangleRounded(itemRect, 0.15f, 8, bgColor);
        DrawRectangleRoundedLines(itemRect, 0.15f, 8, borderColor);

        std::string rankStr = std::to_string(i + 4);
        Vector2 rankSize = MeasureTextEx(font, rankStr.c_str(), 24, 1);
        Vector2 rankPos = {itemRect.x + 10, itemRect.y + (itemHeight - rankSize.y) / 2};
        DrawTextEx(font, rankStr.c_str(), rankPos, 24, 1, Fade(Color{107, 114, 128, 255}, 1.0f));

        Vector2 nameSize = MeasureTextEx(font, entries[i].name.c_str(), 24, 1);
        Vector2 namePos = {itemRect.x + 50, itemRect.y + (itemHeight - nameSize.y) / 2};
        DrawTextEx(font, entries[i].name.c_str(), namePos, 24, 1, WHITE);

        std::string timeStr = FormatTime(entries[i].timeSeconds);
        Vector2 timeSize = MeasureTextEx(font, timeStr.c_str(), 24, 1);
        Vector2 timePos = {itemRect.x + itemRect.width - timeSize.x - 20, itemRect.y + (itemHeight - timeSize.y) / 2};
        DrawTextEx(font, timeStr.c_str(), timePos, 24, 1, Color{147, 197, 253, 255});
    }
}

// Main render function
void RenderLeaderboardScreen(Font &font, int screenWidth, int screenHeight) {
    DrawBackgroundGradient(screenWidth, screenHeight);
    DrawGlowingTitle(font, "LEADERBOARD", screenWidth);

    // Fetch backend players
    std::vector<PlayerData> players = LeaderBoard::fetchTopPlayers("http://localhost:18080/playersInfo");

    // Sort by best time
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

    // Podium positions
    float podiumWidth = screenWidth / 5.0f;
    float podiumBaseY = 140.0f;
    float podiumHeight1 = screenHeight * 0.30f;
    float podiumHeight2 = screenHeight * 0.24f;
    float podiumHeight3 = screenHeight * 0.18f;
    float centerX = screenWidth / 2.0f;
    float padding = 20.0f;

    Rectangle podiumRects[3];
    podiumRects[0] = Rectangle{(float)(centerX - podiumWidth / 2.0f), (float)podiumBaseY, (float)podiumWidth, (float)podiumHeight1};
    podiumRects[1] = Rectangle{(float)(centerX - 1.5f * podiumWidth), (float)(podiumBaseY + (podiumHeight1 - podiumHeight2)), (float)podiumWidth, (float)podiumHeight2};
    podiumRects[2] = Rectangle{(float)(centerX + 0.5f * podiumWidth), (float)(podiumBaseY + (podiumHeight1 - podiumHeight3)), (float)podiumWidth, (float)podiumHeight3};

    // Draw podium enclosure
    float enclosureX = podiumRects[1].x - padding;
    float enclosureY = podiumRects[0].y - padding;
    float enclosureW = (podiumRects[2].x + podiumRects[2].width + padding) - enclosureX;
    float enclosureH = (podiumRects[2].y + podiumRects[2].height + padding) - enclosureY;

    Rectangle podiumEnclosureRect = {enclosureX, enclosureY, enclosureW, enclosureH};
    DrawRectangleRoundedLines(podiumEnclosureRect, 0.1f, 8, Color{210, 180, 140, 255});

    // Draw top 3
    DrawPodiumBlock(1, entries[0], podiumRects[0], font);
    DrawPodiumBlock(2, entries[1], podiumRects[1], font);
    DrawPodiumBlock(3, entries[2], podiumRects[2], font);

    // Draw others
    float rankedStartY = podiumEnclosureRect.y + podiumEnclosureRect.height + 30;
    std::vector<LeaderboardEntry> others(entries.begin() + 3, entries.end());

    float listHeight = (50 + 12) * (int)others.size() - 12;
    float listWidth = screenWidth * 0.8f;
    float listX = (screenWidth - listWidth) / 2.0f;

    Rectangle listEnclosureRect = {
        (float)(listX - padding),
        (float)(rankedStartY - padding),
        (float)(listWidth + padding * 2),
        (float)(listHeight + padding * 2)
    };

    DrawRectangleRoundedLines(listEnclosureRect, 0.1f, 8, Color{210, 180, 140, 255});
    DrawRankedList(others, font, screenWidth, (int)rankedStartY);
}
