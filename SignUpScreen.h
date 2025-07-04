#ifndef SIGNUPSCREEN_H//if not define yet
#define SIGNUPSCREEN_H//define it now

#include "raylib.h"
#include <string>

class SignUpScreen {
public:
    SignUpScreen(int screenWidth, int screenHeight);

    void Update();
    void Draw();

    // For screen switching signals
    bool IsSignInClicked() const;
    void ResetSignInClick();

    bool IsAccountCreated() const;
    void ResetAccountCreated();

private:
    int screenWidth;
    int screenHeight;

    // Input focus states
    bool fullNameFocused;
    bool emailFocused;
    bool passwordFocused;
    bool passwordVisible;
     bool showResponse;
  double responseStartTime;
  char responseMessage[128];

    // Input text buffers
    char fullName[128];
    char emailText[128];
    char passwordText[128];

    int fullNameLen;
    int emailLen;
    int passwordLen;

    // Cursor blink control
    double cursorTimer;
    bool cursorVisible;

    // Error flags and messages
    bool showError;
    char fullNameError[64];
    char emailError[64];
    char passwordError[64];

    // Button click flags
    bool signInClicked;
    bool accountCreated;

    // Textures and font
    Texture2D background;
    Texture2D circle;
    Texture2D login;
    Texture2D user;
    Texture2D emailIcon;
    Texture2D lockIcon;
    Texture2D eyeIcon;
    Font font;

    // Private helper functions
    bool IsValidEmail(const std::string &email);
    bool IsValidPassword(const char *password);
};

#endif
