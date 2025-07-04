
//login screen
#include "LoginScreen.h"
#include <cstring>
#include "login_class.h"
#include <cctype>
#include <iostream>
// Constructor initializes screen size, state flags, text buffers, and loads resources
LoginScreen::LoginScreen(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight),
    emailFocused(false), passwordFocused(false), passwordVisible(false),showResponse(false),responseStartTime(0),responseMessage(""),
    emailLen(0), passwordLen(0), cursorTimer(0), cursorVisible(true),
    showError(false), registerClicked(false), loggedIn(false)
{
    // Initialize input and error strings
    emailText[0] = passwordText[0] = '\0';
    emailError[0] = passwordError[0] = '\0';
//load textures and fonts
  background = LoadTexture("../assets/images/background.jpg");
circle = LoadTexture("../assets/images/circle.png");
login = LoadTexture("../assets/images/login.png");
user = LoadTexture("../assets/images/user.png");
emailIcon = LoadTexture("../assets/images/email.png");
lockIcon = LoadTexture("../assets/images/lock.png");
eyeIcon = LoadTexture("../assets/images/eyes.png");
font = LoadFontEx("D:/Anonymous/Developer/MyRaylib/MineSweeper/hello_raylib/assets/fonts/Poppins/Poppins/Poppins-SemiBold.ttf", 48, 0, 250);

}
// Flags for screen switching
bool LoginScreen::IsRegisterClicked() const { return registerClicked; }
void LoginScreen::ResetRegisterClick() { registerClicked = false; }

bool LoginScreen::IsLoggedIn() const { return loggedIn; }
void LoginScreen::ResetLoggedIn() { loggedIn = false; }
// Update method handles mouse clicks, text input, backspace, and cursor blinking
void LoginScreen::Update()
{
    Vector2 mouse = GetMousePosition();
// Mouse button interaction logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Focus input fields based on click
        Rectangle emailRect = {40, 490, (float)(screenWidth - 80), 70};
        Rectangle passwordRect = {40, 610, (float)(screenWidth - 80), 70};

        emailFocused = CheckCollisionPointRec(mouse, emailRect);
        passwordFocused = CheckCollisionPointRec(mouse, passwordRect);
// Toggle password visibility if eye icon is clicked
        Rectangle eyeRect = {690, 620, 32, 32};
        if (CheckCollisionPointRec(mouse, eyeRect))
            passwordVisible = !passwordVisible;
 // Switch to register screen if "Register" clicked
        Rectangle registerTextRect = {470, 900, 120, 30};
        if (CheckCollisionPointRec(mouse, registerTextRect))
            registerClicked = true;
// Login validation on button click
        Rectangle loginButtonRect = {40, 745, (float)(screenWidth - 80), 65};
        if (CheckCollisionPointRec(mouse, loginButtonRect))
        {
            showError = true;
            if (strlen(emailText) == 0)
                strcpy(emailError, "Please enter email.");
            else
                emailError[0] = '\0';

            if (strlen(passwordText) < 6)
                strcpy(passwordError, "Password must contain uppercase,lowercase,and number.");
            else
                passwordError[0] = '\0';

            if (emailError[0] == '\0' && passwordError[0] == '\0')
      {
                    Login add(emailText, passwordText);
          std::string response = add.sendRequest();
          std::cout << response << std::endl;

          strncpy(responseMessage, response.c_str(), sizeof(responseMessage) - 1);
          responseMessage[sizeof(responseMessage) - 1] = '\0';

          showResponse = true;
          responseStartTime = GetTime();
          if (response.compare("Login Successful !") == 0)
{
                loggedIn=true;
          }
                //accountCreated = true;
            }
        }
    }
 // Handle keyboard input characters
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            if (emailFocused && emailLen < 127)
            {
                emailText[emailLen++] = (char)key;
                emailText[emailLen] = '\0';
            }
            else if (passwordFocused && passwordLen < 127)
            {
                passwordText[passwordLen++] = (char)key;
                passwordText[passwordLen] = '\0';
            }
        }
        key = GetCharPressed();
    }

 // Handle backspace for deleting characters
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (emailFocused && emailLen > 0)
            emailText[--emailLen] = '\0';
        else if (passwordFocused && passwordLen > 0)
            passwordText[--passwordLen] = '\0';
    }
// Cursor blinking logic
    cursorTimer += GetFrameTime();
    if (cursorTimer >= 0.5f)
    {
        cursorVisible = !cursorVisible;
        cursorTimer = 0;
    }
}
// Drawing the entire Login UI
void LoginScreen::Draw()
{
    //BeginDrawing();
    ClearBackground(RAYWHITE);
// Draw background and header graphics
    DrawTexturePro(background, Rectangle{0, 0, (float)background.width, (float)background.height},
                   Rectangle{0, 0, (float)screenWidth, (float)screenHeight}, Vector2{0, 0}, 0, WHITE);
    DrawTexture(circle, 430, -250, WHITE);
    DrawTextureEx(login, Vector2{200, 50}, 0.0f, 0.62f, WHITE);
// Draw main white rounded form background
DrawRectangleRounded(Rectangle{0.0f, 350.0f, (float)screenWidth, (float)(screenHeight - 100)}, 0.1f, 20, WHITE);

    DrawTextEx(font, " LOG IN ", Vector2{350, 350}, 45.0f, 2.0f, BLUE);

    // Email Field
    DrawTextEx(font, "Email Address", Vector2{50, 435}, 25.0f, 2.0f, BLACK);
    // Blinking cursor in email
    if (emailFocused)
        DrawRectangleRounded(Rectangle{38.0f, 488.0f, (float)(screenWidth - 76), 74.0f}, 0.4f, 20, BLUE);
    DrawRectangleRounded(Rectangle{40, 490, (float)(screenWidth - 80), 70}, 0.4f, 20, Color{216, 240, 251, 204});
    DrawTextureEx(emailIcon, Vector2{50, 500}, 0.0f, 0.13f, WHITE);
    DrawTextEx(font, emailLen > 0 ? emailText : "Email Address", Vector2{130, 510}, 25.0f, 2.0f, emailLen > 0 ? BLACK : GRAY);
    if (emailFocused && cursorVisible)
    {
        float textWidth = MeasureTextEx(font, emailText, 25.0f, 2.0f).x;
        DrawTextEx(font, "|", Vector2{130 + textWidth + 2, 510}, 25.0f, 2.0f, BLACK);
    }
    // Email validation error
    if (showError && strlen(emailError) > 0)
        DrawTextEx(font, emailError, Vector2{50, 565}, 18.0f, 1.0f, RED);

    // Password Field
    DrawTextEx(font, "Password", Vector2{50, 585}, 25.0f, 2.0f, BLACK);
    if (passwordFocused)
        DrawRectangleRounded(Rectangle{38.0f, 608.0f, (float)(screenWidth - 76), 74.0f}, 0.4f, 20, BLUE);
    DrawRectangleRounded(Rectangle{40, 610, (float)(screenWidth - 80), 70}, 0.4f, 20, Color{216, 240, 251, 204});
    // Icons: Lock and Eye
    DrawTextureEx(lockIcon, Vector2{50, 620}, 0.0f, 0.099f, WHITE);
    DrawTextureEx(eyeIcon, Vector2{680, 620}, 0.0f, 0.099f, BLACK);
    // Draw strikethrough eye if password is hidden
    if (!passwordVisible)
      // DrawLineEx(Vector2{690, 630}, Vector2{720, 660}, 3.0f, BLACK);
       DrawLineEx(Vector2{715, 633}, Vector2{695, 656}, 3.0f, BLACK);

// Display either password or masked text
    if (passwordLen > 0)
    {
        if (passwordVisible)
        {
            DrawTextEx(font, passwordText, Vector2{130, 630}, 25.0f, 2.0f, BLACK);
            if (passwordFocused && cursorVisible)
            {
                float textWidth = MeasureTextEx(font, passwordText, 25.0f, 2.0f).x;
                DrawTextEx(font, "|", Vector2{130 + textWidth + 2, 630}, 25.0f, 2.0f, BLACK);
            }
        }
        else
        {
            // Create * mask
            char hidden[128] = "";
            for (int i = 0; i < passwordLen; i++) hidden[i] = '*';
            hidden[passwordLen] = '\0';
            DrawTextEx(font, hidden, Vector2{130, 630}, 35.0f, 2.0f, BLACK);
            if (passwordFocused && cursorVisible)
            {
                float textWidth = MeasureTextEx(font, hidden, 35.0f, 2.0f).x;
                DrawTextEx(font, "|", Vector2{130 + textWidth + 2, 630}, 25.0f, 2.0f, BLACK);
            }
        }
    }
    else
    {
        DrawTextEx(font, "Password", Vector2{130, 630}, 25.0f, 2.0f, GRAY);
        if (passwordFocused && cursorVisible)
            DrawTextEx(font, "|", Vector2{132, 630}, 25.0f, 2.0f, BLACK);
    }
     // Password error
    if (showError && strlen(passwordError) > 0)
        DrawTextEx(font, passwordError, Vector2{50, 685}, 18.0f, 1.0f, RED);

    // Login Button
    DrawRectangleRounded(Rectangle{40, 745, (float)(screenWidth - 80), 65}, 0.4f, 20, DARKBLUE);
    DrawTextEx(font, "Log In", Vector2{370, 760}, 30.0f, 1.0f, WHITE);

     DrawTextEx(font, "OR", Vector2{390, 850}, 30.0f, 1.0f, BLACK);

    // Footer
    DrawTextEx(font, "Don't have an account?", Vector2{270, 900}, 20.0f, 1.0f, BLACK);
    DrawTextEx(font, "Register", Vector2{470, 900}, 20.0f, 1.0f, DARKPURPLE);
     //Toast

    if (showResponse && GetTime() - responseStartTime < 3.0)
    {
      DrawRectangleRounded(Rectangle{200, 835, 380, 50}, 0.6f, 10, Color{39, 120, 245, 255});

      float textWidth = MeasureTextEx(font, responseMessage, 25, 1.0f).x;

      float x = 200 + (400 - textWidth) / 2;
      if (strcmp(responseMessage, "User Already Exist") == 0)
        x += 10;

      Vector2 textPos = {x, 850};
      DrawTextEx(font, responseMessage, textPos, 25, 1.0f, WHITE);
    }
    else
    {
      showResponse = false;
    }

    //EndDrawing();
}
