#include "home_screen.h"
#include <raylib.h>

// -------- Button class --------

Button::Button(float x, float y, float w, float h, const std::string &label)
    : rect({x, y, w, h}), text(label), hovered(false)
{
    baseColor = {40, 50, 70, 255};   // Deep blue-gray
    hoverColor = {60, 80, 120, 255}; // Lighter blue on hover
    textColor = RAYWHITE;
}

void Button::Draw(Font *font)
{
    Color colorToDraw = hovered ? hoverColor : baseColor;
    float yOffset = hovered ? 2.0f : 0.0f;
    Rectangle drawRect = rect;
    drawRect.y += yOffset;

    DrawRectangleRounded(drawRect, 0.18f, 12, colorToDraw);

    if (hovered)
    {
        for (int i = 0; i < 4; i++)
            DrawRectangleRoundedLinesEx(drawRect, 0.18f, 12, 3 + i, BLACK);
    }
    else
    {
        DrawRectangleRoundedLines(drawRect, 0.18f, 12, BLACK);
    }

    int fontSize = (int)(rect.height * 0.48f);
    Vector2 textPos;

    if (font && font->texture.id != 0)
    {
        Vector2 size = MeasureTextEx(*font, text.c_str(), fontSize, 1);
        textPos.x = rect.x + (rect.width - size.x) / 2;
        textPos.y = rect.y + (rect.height - size.y) / 2 + yOffset;
        DrawTextEx(*font, text.c_str(), textPos, fontSize, 1, textColor);
    }
    else
    {
        int textWidth = MeasureText(text.c_str(), fontSize);
        int textX = (int)(rect.x + (rect.width - textWidth) / 2);
        int textY = (int)(rect.y + (rect.height - fontSize) / 2 + yOffset);
        DrawText(text.c_str(), textX, textY, fontSize, textColor);
    }
}

bool Button::Update(Vector2 mousePos, bool mousePressed)
{
    hovered = CheckCollisionPointRec(mousePos, rect);
    return hovered && mousePressed;
}

// -------- UIManager class --------

void UIManager::Init()
{
    customFont = LoadFontEx("resources/fonts/Merriweather_120pt-Regular.ttf", 64, 0, 0);
    homeIcon = LoadTexture("resources/minesweeper_assets/images/home_icon.png");

    // Home buttons
    float btnW = 320, btnH = 56, btnX = 290;
    float startY = 340, gap = 24;
    homeButtons.clear();
    homeButtons.emplace_back(btnX, startY, btnW, btnH, "NEW GAME");
    homeButtons.emplace_back(btnX, startY + (btnH + gap) * 1, btnW, btnH, "OPTIONS");
    homeButtons.emplace_back(btnX, startY + (btnH + gap) * 2, btnW, btnH, "LEADERBOARD");
    homeButtons.emplace_back(btnX, startY + (btnH + gap) * 3, btnW, btnH, "HOW TO PLAY");
    homeButtons.emplace_back(btnX, startY + (btnH + gap) * 4, btnW, btnH, "EXIT");

    // Options buttons
    optionsButtons.clear();
    optionsButtons.emplace_back(300, 240, 380, 60, "Toggle Sound");
    optionsButtons.emplace_back(300, 320, 380, 60, "Toggle Music");
    optionsButtons.emplace_back(300, 400, 380, 60, "Back");

    // Level selection buttons
    levelButtons.clear();
    levelButtons.emplace_back(btnX, 220, btnW, btnH, "Easy (12x12)");
    levelButtons.emplace_back(btnX, 300, btnW, btnH, "Medium (15x15)");
    levelButtons.emplace_back(btnX, 380, btnW, btnH, "Hard (20x20)");
}

void UIManager::Unload()
{
    if (customFont.texture.id != 0)
        UnloadFont(customFont);
    if (homeIcon.id != 0)
        UnloadTexture(homeIcon);
}

int UIManager::UpdateHome(Vector2 mousePos, bool mousePressed)
{
    for (int i = 0; i < (int)homeButtons.size(); i++)
    {
        if (homeButtons[i].Update(mousePos, mousePressed))
        {
            if (i == 4) // Exit button
                CloseWindow();
            return i;
        }
    }
    return -1;
}

void UIManager::DrawHome()
{
    ClearBackground({22, 28, 40, 255});
    DrawTextEx(customFont, "MINESWEEPER", {180, 80}, 72, 1, RAYWHITE);

    float iconW = 90, iconH = 90;
    float iconX = 450 - iconW / 2;
    float iconY = 200;
    if (homeIcon.id != 0)
        DrawTexturePro(homeIcon, {0, 0, (float)homeIcon.width, (float)homeIcon.height},
                       {iconX, iconY, iconW, iconH}, {0, 0}, 0, WHITE);

    for (auto &btn : homeButtons)
        btn.Draw(&customFont);
}

int UIManager::UpdateOptions(Vector2 mousePos, bool mousePressed)
{
    for (int i = 0; i < (int)optionsButtons.size(); i++)
    {
        if (optionsButtons[i].Update(mousePos, mousePressed))
            return i;
    }
    return -1;
}

void UIManager::DrawOptions(bool soundOn, bool musicOn)
{
    ClearBackground({22, 28, 40, 255});
    DrawTextEx(customFont, "OPTIONS", {320, 120}, 64, 1, RAYWHITE);

    for (int i = 0; i < (int)optionsButtons.size(); i++)
    {
        Button &btn = optionsButtons[i];
        btn.Draw(&customFont);
        if (i == 0)
        {
            DrawText(soundOn ? "ON" : "OFF", (int)(btn.rect.x + btn.rect.width - 60), (int)(btn.rect.y + 15), 36, soundOn ? GREEN : RED);
        }
        else if (i == 1)
        {
            DrawText(musicOn ? "ON" : "OFF", (int)(btn.rect.x + btn.rect.width - 60), (int)(btn.rect.y + 15), 36, musicOn ? GREEN : RED);
        }
    }
}

int UIManager::UpdateLevelSelection(Vector2 mousePos, bool mousePressed)
{
    for (int i = 0; i < (int)levelButtons.size(); i++)
    {
        if (levelButtons[i].Update(mousePos, mousePressed))
            return i;
    }
    return -1;
}

void UIManager::DrawLevelSelection()
{
    ClearBackground({22, 28, 40, 255});
    DrawTextEx(customFont, "Select Difficulty", {270, 120}, 56, 1, RAYWHITE);
    for (auto &btn : levelButtons)
        btn.Draw(&customFont);
}

void UIManager::DrawHowToPlay()
{
    ClearBackground({220, 220, 220, 255});
    DrawTextEx(customFont, "HOW TO PLAY", {320, 80}, 64, 1, BLACK);

    Rectangle box = {100, 160, 700, 400};
    DrawRectangleRounded(box, 0.15f, 12, {30, 40, 60, 220});
    DrawRectangleRoundedLines(box, 0.15f, 12, BLACK);

    struct Instruction
    {
        const char *text;
        int icon; // 0 = bomb, 1 = flag, -1 = no icon
    };

    Instruction instructions[] = {
        {"Left Click: Reveal tile", 0},
        {"Right Click: Flag tile", 1},
        {"Reveal all safe tiles to win.", -1},
        {"Avoid the mines!", -1},
        {"", -1},
        {"Press ENTER to return to the main menu.", -1}};

    static Texture2D icons[2] = {
        LoadTexture("resources/minesweeper_assets/images/bomb.png"),
        LoadTexture("resources/minesweeper_assets/images/flag.png")};

    const float iconSize = 32.0f;
    const float iconPadding = 12.0f;
    int startX = (int)box.x + 40;
    int startY = (int)box.y + 40;
    int lineHeight = 50;

    for (int i = 0; i < (int)(sizeof(instructions) / sizeof(instructions[0])); i++)
    {
        int y = startY + i * lineHeight;

        if (instructions[i].icon >= 0)
        {
            float iconY = y + (lineHeight - iconSize) / 2.0f;
            Rectangle sourceRec = {0, 0, (float)icons[instructions[i].icon].width, (float)icons[instructions[i].icon].height};
            Rectangle destRec = {(float)startX, iconY, iconSize, iconSize};
            Vector2 origin = {0, 0};
            DrawTexturePro(icons[instructions[i].icon], sourceRec, destRec, origin, 0.0f, WHITE);

            float textX = startX + iconSize + iconPadding;
            DrawTextEx(customFont, instructions[i].text, {textX, (float)y}, 30, 1, BLACK);
        }
        else
        {
            DrawTextEx(customFont, instructions[i].text, {(float)startX, (float)y}, 30, 1, BLACK);
        }
    }

    // Note: UnloadTexture(icons[0]) and UnloadTexture(icons[1]) should be called in UIManager::Unload()
}