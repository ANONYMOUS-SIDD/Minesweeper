#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include <raylib.h>
#include <string>
#include <vector>

class Button
{
public:
    Rectangle rect;
    std::string text;
    bool hovered;
    Color baseColor;
    Color hoverColor;
    Color textColor;

    Button(float x, float y, float w, float h, const std::string &label);

    void Draw(Font *font = nullptr);
    bool Update(Vector2 mousePos, bool mousePressed);
};

class UIManager
{
public:
    std::vector<Button> homeButtons;
    std::vector<Button> optionsButtons;
    std::vector<Button> levelButtons;
    Font customFont;
    Texture2D homeIcon;

    void Init();
    void Unload();

    int UpdateHome(Vector2 mousePos, bool mousePressed);
    void DrawHome();

    int UpdateOptions(Vector2 mousePos, bool mousePressed);
    void DrawOptions(bool soundOn, bool musicOn);

    int UpdateLevelSelection(Vector2 mousePos, bool mousePressed);
    void DrawLevelSelection();

    void DrawHowToPlay();
};

#endif // HOME_SCREEN_H
