#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include <raylib.h>
#include <string>
#include <vector>

// --- Button class ---



class Button {
public:
    Rectangle rect;
    std::string text;
    Color baseColor;
    Color hoverColor;
    Color textColor;
    bool hovered;
    Texture2D icon;

    // Constructor with icon
    Button(float x, float y, float w, float h, const std::string& label, const std::string& iconPath);

    // Constructor without icon
    Button(float x, float y, float w, float h, const std::string& label);

    void Draw(Font *font);
    bool Update(Vector2 mousePos, bool mousePressed);
};




class UIManager
{
public:
    std::vector<Button> homeButtons;
    std::vector<Button> optionsButtons;
    std::vector<Button> levelButtons;
    std::string email;
    Font customFont;
    Texture2D homeIcon;
    Texture2D background;
      Texture2D corner;
        Texture2D corner_bottom;
        Texture2D New_Game;
         Texture2D mineBg;
          Texture2D lines;
          Texture2D notification;
            Texture2D user;
            Texture2D border;
              Texture2D avatar;


    void Init();
    void Unload();
    void setEmail(std::string email);

    int UpdateHome(Vector2 mousePos, bool mousePressed);
    void DrawHome();

    int UpdateOptions(Vector2 mousePos, bool mousePressed);
    void DrawOptions(bool soundOn, bool musicOn);

    int UpdateLevelSelection(Vector2 mousePos, bool mousePressed);
    void DrawLevelSelection();
    static void setEmails(std::string emails);

    void DrawHowToPlay();
};

#endif // HOME_SCREEN_H
