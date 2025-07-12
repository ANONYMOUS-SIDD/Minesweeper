#include "leaderboard.h"
#include <raylib.h>
#include <raymath.h>
#include <string>

// Define the leaderboard entries
LeaderboardEntry top3[3] = {
    {"Prassiddha", 58 * 60 + 12},
    {"Siddhant", 65 * 60 + 33},
    {"Arip", 70 * 60 + 2}};

LeaderboardEntry others[2] = {
    {"Sushant", 75 * 60 + 42},
    {"PlayerX", 88 * 60 + 19}};

static Color LerpColor(Color a, Color b, float t)
{
    Color result;
    result.r = (unsigned char)(a.r + (b.r - a.r) * t);
    result.g = (unsigned char)(a.g + (b.g - a.g) * t);
    result.b = (unsigned char)(a.b + (b.b - a.b) * t);
    result.a = (unsigned char)(a.a + (b.a - a.a) * t);
    return result;
}

std::string FormatTime(float seconds)
{
    int mins = (int)(seconds / 60);
    int secs = (int)seconds % 60;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", mins, secs);
    return std::string(buffer);
}

void DrawBackgroundGradient(int width, int height)
{
    for (int y = 0; y < height; y += 4)
    {
        float t = (float)y / height;
        Color c = LerpColor(Color{30, 30, 40, 255}, Color{15, 15, 20, 255}, t);
        DrawRectangle(0, y, width, 4, c);
    }
}

void DrawGlowingTitle(Font &font, const char *text, int screenWidth)
{
    int fontSize = 72;
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 2);
    Vector2 pos = {(screenWidth - textSize.x) / 2, 40};

    for (int i = 4; i >= 1; i--)
    {
        float alpha = 0.1f * i;
        DrawTextEx(font, text, pos, fontSize, 2, Fade((Color){100, 150, 255, 255}, alpha));
    }
    DrawTextEx(font, text, pos, fontSize, 2, WHITE);
}

void DrawPodiumBlock(int rank, const LeaderboardEntry &entry, Rectangle rect, Font &font)
{
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

void DrawRankedList(const LeaderboardEntry entries[], int count, Font &font, int screenWidth, int startY)
{
    int itemHeight = 50;
    int padding = 12;
    int listWidth = screenWidth * 0.8f;
    int startX = (screenWidth - listWidth) / 2;

    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < count; i++)
    {
        Rectangle itemRect = {(float)startX, (float)(startY + i * (itemHeight + padding)), (float)listWidth, (float)itemHeight};

        bool hovered = CheckCollisionPointRec(mousePos, itemRect);
        Color bgColor = hovered ? Fade((Color){55, 65, 81, 255}, 0.9f) : Fade((Color){55, 65, 81, 255}, 0.7f);
        Color borderColor = hovered ? (Color){59, 130, 246, 255} : (Color){55, 65, 81, 255};

        DrawRectangleRounded(itemRect, 0.15f, 8, bgColor);
        DrawRectangleRoundedLines(itemRect, 0.15f, 8, borderColor);

        std::string rankStr = std::to_string(i + 4);
        Vector2 rankSize = MeasureTextEx(font, rankStr.c_str(), 24, 1);
        Vector2 rankPos = {itemRect.x + 10, itemRect.y + (itemHeight - rankSize.y) / 2};
        DrawTextEx(font, rankStr.c_str(), rankPos, 24, 1, Fade((Color){107, 114, 128, 255}, 1.0f));

        Vector2 nameSize = MeasureTextEx(font, entries[i].name.c_str(), 24, 1);
        Vector2 namePos = {itemRect.x + 50, itemRect.y + (itemHeight - nameSize.y) / 2};
        DrawTextEx(font, entries[i].name.c_str(), namePos, 24, 1, WHITE);

        std::string timeStr = FormatTime(entries[i].timeSeconds);
        Vector2 timeSize = MeasureTextEx(font, timeStr.c_str(), 24, 1);
        Vector2 timePos = {itemRect.x + itemRect.width - timeSize.x - 20, itemRect.y + (itemHeight - timeSize.y) / 2};
        DrawTextEx(font, timeStr.c_str(), timePos, 24, 1, (Color){147, 197, 253, 255});
    }
}

void RenderLeaderboardScreen(Font &font, int screenWidth, int screenHeight)
{
    DrawBackgroundGradient(screenWidth, screenHeight);
    DrawGlowingTitle(font, "LEADERBOARD", screenWidth);

    float podiumWidth = screenWidth / 5.0f;
    float podiumBaseY = 140;
    float podiumHeight1 = screenHeight * 0.30f;
    float podiumHeight2 = screenHeight * 0.24f;
    float podiumHeight3 = screenHeight * 0.18f;
    float centerX = screenWidth / 2.0f;

    Rectangle podiumRects[3] = {
        {centerX - podiumWidth / 2.0f, podiumBaseY, podiumWidth, podiumHeight1},
        {centerX - podiumWidth / 2.0f - podiumWidth, podiumBaseY + (podiumHeight1 - podiumHeight2), podiumWidth, podiumHeight2},
        {centerX + podiumWidth / 2.0f, podiumBaseY + (podiumHeight1 - podiumHeight3), podiumWidth, podiumHeight3}};

    float padding = 20;

    float podiumEnclosureX = podiumRects[1].x - padding;
    float podiumEnclosureY = podiumRects[0].y - padding;
    float podiumEnclosureW = (podiumRects[2].x + podiumRects[2].width + padding) - podiumEnclosureX;
    float podiumEnclosureH = (podiumRects[2].y + podiumRects[2].height + padding) - podiumEnclosureY;

    Rectangle podiumEnclosureRect = {podiumEnclosureX, podiumEnclosureY, podiumEnclosureW, podiumEnclosureH};

    DrawRectangleRoundedLines(podiumEnclosureRect, 0.1f, 8, Color{210, 180, 140, 255});

    DrawPodiumBlock(1, top3[0], podiumRects[0], font);
    DrawPodiumBlock(2, top3[1], podiumRects[1], font);
    DrawPodiumBlock(3, top3[2], podiumRects[2], font);

    float rankedListStartY = podiumEnclosureRect.y + podiumEnclosureRect.height + 30;
    float itemHeight = 50;
    float itemPadding = 12;
    float listHeight = (itemHeight + itemPadding) * 2 - itemPadding;
    float listWidth = screenWidth * 0.8f;
    float listX = (screenWidth - listWidth) / 2;

    Rectangle listEnclosureRect = {
        listX - padding,
        rankedListStartY - padding,
        listWidth + padding * 2,
        listHeight + padding * 2};

    DrawRectangleRoundedLines(listEnclosureRect, 0.1f, 8, Color{210, 180, 140, 255});
    DrawRankedList(others, 2, font, screenWidth, rankedListStartY);
}
