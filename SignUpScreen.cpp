//signup screen

#include "SignUpScreen.h"
// Header File To Include All The Necessary Resources
#include "raylib.h"
#include <regex>
#include <stdbool.h>
#include <string.h>
#include "signup_class.h"
#include <iostream>
#include <ctype.h>
#include"score_class.h"

// Constructor: Load resources and initialize variables
SignUpScreen::SignUpScreen(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight),
    fullNameFocused(false), emailFocused(false), passwordFocused(false), passwordVisible(false),showResponse(false),responseStartTime(0),responseMessage(""),
    fullNameLen(0), emailLen(0), passwordLen(0),
    cursorTimer(0.0), cursorVisible(true), showError(false),
    signInClicked(false), accountCreated(false)
{
    fullName[0] = emailText[0] = passwordText[0] = '\0';
    fullNameError[0] = emailError[0] = passwordError[0] = '\0';

   background = LoadTexture("../assets/images/background.jpg");
circle = LoadTexture("../assets/images/circle.png");
login = LoadTexture("../assets/images/login.png");
user = LoadTexture("../assets/images/userProfile.png");
emailIcon = LoadTexture("../assets/images/email.png");
lockIcon = LoadTexture("../assets/images/lock.png");
eyeIcon = LoadTexture("../assets/images/eyes.png");
font = LoadFontEx("D:/Anonymous/Developer/MyRaylib/MineSweeper/hello_raylib/assets/fonts/Poppins/Poppins/Poppins-SemiBold.ttf", 48, 0, 250);


}

// Validate email with regex
bool SignUpScreen::IsValidEmail(const std::string &email)
{
    const std::regex pattern("^[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,}$");
    return std::regex_match(email, pattern);
}

// Validate password requirements
bool SignUpScreen::IsValidPassword(const char *password)
{
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (int i = 0; password[i] != '\0'; i++)
    {
        if (isupper(password[i])) hasUpper = true;
        else if (islower(password[i])) hasLower = true;
        else if (isdigit(password[i])) hasDigit = true;
    }
    return hasUpper && hasLower && hasDigit && strlen(password) >= 6;
}

// Reset SignIn clicked flag
void SignUpScreen::ResetSignInClick() { signInClicked = false; }
// Reset Account Created flag
void SignUpScreen::ResetAccountCreated() { accountCreated = false; }

bool SignUpScreen::IsSignInClicked() const { return signInClicked; }
bool SignUpScreen::IsAccountCreated() const { return accountCreated; }

// Main update logic - handle inputs and states
void SignUpScreen::Update()
{
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Input focus check
        Rectangle full_name = {40.0f, 360.0f, (float)(screenWidth - 80), 70.0f};
        Rectangle email_address = {40.0f, 490.0f, (float)(screenWidth - 80), 70.0f};
        Rectangle password = {40.0f, 610.0f, (float)(screenWidth - 80), 70.0f};

        fullNameFocused = CheckCollisionPointRec(mouse, full_name);
        emailFocused = CheckCollisionPointRec(mouse, email_address);
        passwordFocused = CheckCollisionPointRec(mouse, password);

        // Only one input focused at a time
        if (fullNameFocused) emailFocused = passwordFocused = false;
        else if (emailFocused) fullNameFocused = passwordFocused = false;
        else if (passwordFocused) fullNameFocused = emailFocused = false;

        // Eye icon toggle password visibility
        Rectangle eyeButton = {690.0f, 620.0f, 32.0f, 32.0f};
        if (CheckCollisionPointRec(mouse, eyeButton))
            passwordVisible = !passwordVisible;

        // Sign In text clicked (bottom right text)
        Rectangle signInText = {430.0f, 900.0f, 100.0f, 30.0f}; // approximate clickable area
        if (CheckCollisionPointRec(mouse, signInText))
            signInClicked = true;

        // Create Account button clicked
        Rectangle create_account = {40.0f, 745.0f, (float)(screenWidth - 80), 65.0f};
        if (CheckCollisionPointRec(mouse, create_account))
        {
            showError = true;

            if (strlen(fullName) < 3) strcpy(fullNameError, "Enter at least 3 characters.");
            else strcpy(fullNameError, "");

            if (!IsValidEmail(emailText)) strcpy(emailError, "enter valid email format(e.g., name@gmail.com).");
            else strcpy(emailError, "");

            if (!IsValidPassword(passwordText)) strcpy(passwordError, "Password must contain uppercase, lowercase, number.");
            else strcpy(passwordError, "");

            // Simulate account creation if no errors
            if (strlen(fullNameError) == 0 && strlen(emailError) == 0 && strlen(passwordError) == 0)
            {

                    SignUp add(fullName, emailText, passwordText);
    ;
          std::string response = add.sendRequest();
          std::cout << response << std::endl;


          strncpy(responseMessage, response.c_str(), sizeof(responseMessage) - 1);
          responseMessage[sizeof(responseMessage) - 1] = '\0';

          showResponse = true;
          responseStartTime = GetTime();
                //accountCreated = true;
            }
        }
    }

    // Character input
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            if (fullNameFocused && fullNameLen < 127)
            {
                fullName[fullNameLen++] = (char)key;
                fullName[fullNameLen] = '\0';
            }
            else if (emailFocused && emailLen < 127)
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

    // Backspace handling
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (fullNameFocused && fullNameLen > 0) fullName[--fullNameLen] = '\0';
        else if (emailFocused && emailLen > 0) emailText[--emailLen] = '\0';
        else if (passwordFocused && passwordLen > 0) passwordText[--passwordLen] = '\0';
    }

    // Cursor blinking
    cursorTimer += GetFrameTime();
    if (cursorTimer >= 0.5f)
    {
        cursorVisible = !cursorVisible;
        cursorTimer = 0;
    }
}

// Drawing the entire sign up screen
void SignUpScreen::Draw()
{
    //BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw background, circle, login image
    DrawTexturePro(background, Rectangle{0, 0, (float)background.width, (float)background.height},
                   Rectangle{0, 0, (float)screenWidth, (float)screenHeight}, Vector2{0, 0}, 0, WHITE);
    DrawTexture(circle, 550, -250, WHITE);
    DrawTextureEx(login, Vector2{250, 50}, 0.0f, 0.42f, WHITE);

    // Draw form container
    DrawRectangleRounded(Rectangle{0, 250, (float)screenWidth,(float)(screenHeight - 100)}, 0.1f, 20, WHITE);
    DrawTextEx(font, " Create Account ", Vector2{255, 255}, 40.0f, 2.0f, RED);

    // --- Full Name Field ---
    DrawTextEx(font, "Full Name", Vector2{50, 335}, 25.0f, 2.0f, BLACK);

    if (fullNameFocused)
        DrawRectangleRounded(Rectangle{40 - 2, 360 - 2, (float)(screenWidth - 80) + 4, 70 + 4}, 0.4f, 20, BLUE);
    DrawRectangleRounded(Rectangle{40, 360, (float)(screenWidth - 80), 70}, 0.4f, 20, Color{216, 240, 251, 204});
    DrawTextureEx(user, Vector2{50, 370}, 0.0f, 0.09f, WHITE);
    DrawTextEx(font, fullNameLen > 0 ? fullName : "Full Name", Vector2{130, 380}, 25.0f, 2.0f, fullNameLen > 0 ? BLACK : GRAY);

    if (fullNameFocused && cursorVisible)
    {
        float textWidth = MeasureTextEx(font, fullName, 25.0f, 2.0f).x;
        DrawTextEx(font, "|", Vector2{125 + textWidth + 3, 380}, 25.0f, 2.0f, BLACK);
    }

    if (showError && strlen(fullNameError) > 0)
        DrawTextEx(font, fullNameError, Vector2{50, 430}, 18.0f, 1.0f, RED);

    // --- Email Field ---
    DrawTextEx(font, "Email Address", Vector2{50, 465}, 25.0f, 2.0f, BLACK);
    if (emailFocused)
        DrawRectangleRounded(Rectangle{40 - 2, 490 - 2, (float)(screenWidth - 80) + 4, 70 + 4}, 0.4f, 20, BLUE);
    DrawRectangleRounded(Rectangle{40, 490, (float)(screenWidth - 80), 70}, 0.4f, 20, Color{216, 240, 251, 204});
    DrawTextureEx(emailIcon, Vector2{50, 500}, 0.0f, 0.13f, WHITE);
    DrawTextEx(font, emailLen > 0 ? emailText : "Email Address", Vector2{130, 510}, 25.0f, 2.0f, emailLen > 0 ? BLACK : GRAY);
    if (emailFocused && cursorVisible)
    {
        float textWidth = MeasureTextEx(font, emailText, 25.0f, 2.0f).x;
        DrawTextEx(font, "|", Vector2{125 + textWidth + 3, 510}, 25.0f, 2.0f, BLACK);
    }
    if (showError && strlen(emailError) > 0)
        DrawTextEx(font, emailError, Vector2{50, 560}, 18.0f, 1.0f, RED);

    // --- Password Field ---
    DrawTextEx(font, "Password", Vector2{50, 585}, 25.0f, 2.0f, BLACK);
    if (passwordFocused)
        DrawRectangleRounded(Rectangle{40 - 2, 610 - 2, (float)(screenWidth - 80) + 4, 70 + 4}, 0.4f, 20, BLUE);
    DrawRectangleRounded(Rectangle{40, 610, (float)(screenWidth - 80), 70}, 0.4f, 20, Color{216, 240, 251, 204});
    DrawTextureEx(lockIcon, Vector2{50, 620}, 0.0f, 0.099f, WHITE);
    DrawTextureEx(eyeIcon, Vector2{690, 620}, 0.0f, 0.099f, BLACK);

    if (!passwordVisible)
        DrawLineEx(Vector2{726, 633}, Vector2{703, 656}, 3.0f, BLACK);

    if (passwordLen > 0)
    {
        char hidden[128] = "";
        if (passwordVisible)
        {
            DrawTextEx(font, passwordText, Vector2{130, 630}, 25.0f, 2.0f, BLACK);
            if (passwordFocused && cursorVisible)
            {
                float textWidth = MeasureTextEx(font, passwordText, 25.0f, 2.0f).x;
                DrawTextEx(font, "|", Vector2{125 + textWidth + 3, 630}, 25.0f, 2.0f, BLACK);
            }
        }
        else
        {
            for (int i = 0; i < passwordLen; i++) hidden[i] = '*';
            hidden[passwordLen] = '\0';
            DrawTextEx(font, hidden, Vector2{132, 630}, 35.0f, 2.0f, BLACK);
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
            DrawTextEx(font, "|", Vector2{130 + 2, 630}, 25.0f, 2.0f, BLACK);
    }

    if (showError && strlen(passwordError) > 0)
        DrawTextEx(font, passwordError, Vector2{50, 680}, 18.0f, 1.0f, RED);

    // Draw Create Account button
    DrawRectangleRounded(Rectangle{40, 745, (float)(screenWidth - 80), 65}, 0.4f, 20, DARKBLUE);
    DrawTextEx(font, "Create Account", Vector2{300, 762}, 30.0f, 1.0f, WHITE);

    // Draw "OR" and Sign In prompt
    DrawTextEx(font, "OR", Vector2{390, 850}, 30.0f, 1.0f, BLACK);
    DrawTextEx(font, "Already Have An Account ?", Vector2{270, 900}, 20.0f, 1.0f, BLACK);
    DrawTextEx(font, "Sign In", Vector2{490, 900}, 20.0f, 1.0f, DARKPURPLE);

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


           Score create(fullName,emailText,0,0,0);
          std::string result=create.initRequest();


    }
    else
    {
      showResponse = false;
    }


    //EndDrawing();
}
