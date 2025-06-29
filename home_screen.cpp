#include <raylib.h>
#include <string>
#include <vector>
using namespace std;

struct Button
{
    Rectangle rect;
    string text;
    bool hovered = false;
    Color baseColor;
    Color hoverColor;
    Color textColor;

    Button(float x, float y, float w, float h, const string &label)
        : rect({x, y, w, h}), text(label)
    {
        baseColor = {180, 180, 180, 255};  // Dark gray
        hoverColor = {200, 200, 200, 255}; // Lighter on hover
        textColor = BLACK;                 // Black text
    }

    void Draw(Font *font = nullptr)
    {
        Color colorToDraw = hovered ? hoverColor : baseColor;
        DrawRectangleRounded(rect, 0.25f, 12, colorToDraw);

        int fontSize = (int)(rect.height * 0.5f);
        Vector2 textPos;

        if (font && font->texture.id != 0)
        {
            Vector2 size = MeasureTextEx(*font, text.c_str(), fontSize, 1);
            textPos.x = rect.x + (rect.width - size.x) / 2;
            textPos.y = rect.y + (rect.height - size.y) / 2;
            DrawTextEx(*font, text.c_str(), textPos, fontSize, 1, textColor);
        }
        else
        {
            int textWidth = MeasureText(text.c_str(), fontSize);
            int textX = (int)(rect.x + (rect.width - textWidth) / 2);
            int textY = (int)(rect.y + (rect.height - fontSize) / 2);
            DrawText(text.c_str(), textX, textY, fontSize, textColor);
        }
    }

    bool Update(Vector2 mousePos, bool mousePressed)
    {
        hovered = CheckCollisionPointRec(mousePos, rect);
        return hovered && mousePressed;
    }
};

struct UIManager
{
    vector<Button> homeButtons;
    vector<Button> optionsButtons;
    vector<Button> levelButtons;
    Font customFont;

    void Init()
    {
        // Load Merriweather font
        customFont = LoadFontEx("resources/fonts/Merriweather_120pt-Regular.ttf", 32, 0, 0);

        // Home screen buttons
        homeButtons.push_back(Button(300, 240, 300, 60, "New Game"));
        homeButtons.push_back(Button(300, 320, 300, 60, "Options"));
        homeButtons.push_back(Button(300, 400, 300, 60, "How to Play"));
        homeButtons.push_back(Button(300, 480, 300, 60, "Leaderboard"));
        homeButtons.push_back(Button(300, 560, 300, 60, "Exit"));

        // Options screen buttons
        optionsButtons.push_back(Button(300, 240, 300, 60, "Toggle Sound"));
        optionsButtons.push_back(Button(300, 320, 300, 60, "Toggle Music"));
        optionsButtons.push_back(Button(300, 400, 300, 60, "Back"));

        // Level selection buttons
        levelButtons.push_back(Button(300, 220, 300, 60, "Easy (12x12)"));
        levelButtons.push_back(Button(300, 300, 300, 60, "Medium (15x15)"));
        levelButtons.push_back(Button(300, 380, 300, 60, "Hard (20x20)"));
    }

    void DrawHome()
    {
        ClearBackground({220, 220, 220, 255}); // Light gray background
        DrawTextEx(customFont, "MINESWEEPER", {270, 120}, 50, 1, BLACK);
        for (auto &btn : homeButtons)
            btn.Draw(&customFont);
    }

    void DrawOptions(bool soundOn, bool musicOn)
    {
        ClearBackground({220, 220, 220, 255});
        DrawTextEx(customFont, "OPTIONS", {320, 120}, 50, 1, BLACK);

        for (int i = 0; i < (int)optionsButtons.size(); i++)
        {
            Button &btn = optionsButtons[i];
            btn.Draw(&customFont);
            if (i == 0)
            {
                DrawText(soundOn ? "ON" : "OFF", (int)(btn.rect.x + btn.rect.width - 60), (int)(btn.rect.y + 15), 30, soundOn ? GREEN : RED);
            }
            else if (i == 1)
            {
                DrawText(musicOn ? "ON" : "OFF", (int)(btn.rect.x + btn.rect.width - 60), (int)(btn.rect.y + 15), 30, musicOn ? GREEN : RED);
            }
        }
    }

    void DrawLevelSelection()
    {
        ClearBackground({220, 220, 220, 255});
        DrawTextEx(customFont, "Select Difficulty", {270, 120}, 40, 1, BLACK);
        for (auto &btn : levelButtons)
            btn.Draw(&customFont);
    }

    void DrawHowToPlay()
    {
        ClearBackground({220, 220, 220, 255});
        DrawTextEx(customFont, "HOW TO PLAY", {320, 120}, 50, 1, BLACK);

        const char *instructions[] = {
            "Left Click: Reveal tile",
            "Right Click: Flag tile",
            "Reveal all safe tiles to win.",
            "Avoid the mines!",
            "",
            "Press Enter to go back to the main menu."};

        int startY = 200;
        int lineSpacing = 40;
        for (int i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++)
        {
            DrawTextEx(customFont, instructions[i], {150, (float)(startY + i * lineSpacing)}, 30, 1, DARKGRAY);
        }
    }

    int UpdateHome(Vector2 mousePos, bool mousePressed)
    {
        for (int i = 0; i < (int)homeButtons.size(); i++)
        {
            if (homeButtons[i].Update(mousePos, mousePressed))
                return i;
        }
        return -1;
    }

    int UpdateOptions(Vector2 mousePos, bool mousePressed)
    {
        for (int i = 0; i < (int)optionsButtons.size(); i++)
        {
            if (optionsButtons[i].Update(mousePos, mousePressed))
                return i;
        }
        return -1;
    }

    int UpdateLevelSelection(Vector2 mousePos, bool mousePressed)
    {
        for (int i = 0; i < (int)levelButtons.size(); i++)
        {
            if (levelButtons[i].Update(mousePos, mousePressed))
                return i;
        }
        return -1;
    }

    void Unload()
    {
        if (customFont.texture.id != 0)
            UnloadFont(customFont);
    }
};
