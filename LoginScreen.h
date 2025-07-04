#ifndef LOGINSCREEN_H//if not defined yet
#define LOGINSCREEN_H//define it now

#include "raylib.h"

class LoginScreen {
public:
    LoginScreen(int screenWidth, int screenHeight);

    void Update();
    void Draw();

    // Screen switch flags
    bool IsRegisterClicked() const;
    void ResetRegisterClick();

    bool IsLoggedIn() const;
    void ResetLoggedIn();

private:
    int screenWidth;
    int screenHeight;

    bool emailFocused;
    bool passwordFocused;
    bool passwordVisible;
    bool showResponse;
  double responseStartTime;
  char responseMessage[128];

    char emailText[128];
    char passwordText[128];
    int emailLen;
    int passwordLen;

    double cursorTimer;
    bool cursorVisible;

    bool showError;
    char emailError[128];
    char passwordError[128];

    bool registerClicked;
    bool loggedIn;

    Texture2D background;
    Texture2D circle;
    Texture2D login;
    Texture2D user;
    Texture2D emailIcon;
    Texture2D lockIcon;
    Texture2D eyeIcon;
    Font font;
};

#endif
