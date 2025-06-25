// MineSweeper SignUp Page

// Header File To Include All The Necessary Resources
#include "raylib.h"
#include <regex>
#include <stdbool.h>
#include <string.h>
#include "signup_class.h"
#include <iostream>
#include <ctype.h>

// Function To Validate Email Format
bool IsValidEmail(const std::string &email)
{
  // Use RegX For Email Validation
  const std::regex pattern("^[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,}$");
  return std::regex_match(email, pattern);
}
// Function To Validate Password Requirements
bool IsValidPassword(const char *password)
{
  bool hasUpper = false, hasLower = false, hasDigit = false;
  for (int i = 0; password[i] != '\0'; i++)
  {
    if (isupper(password[i]))
      hasUpper = true;
    else if (islower(password[i]))
      hasLower = true;
    else if (isdigit(password[i]))
      hasDigit = true;
  }
  return hasUpper && hasLower && hasDigit && strlen(password) >= 6;
}
// Main Function To Start Execution
int main()
{
  // Initialize Screen Width And Height
  const int screenWidth = 700;
  const int screenHeight = 970;
  // Initialize Game Window
  InitWindow(screenWidth, screenHeight, "MineSweeper");
  // Load All The Necessary Images From assets Folder
  Texture2D background = LoadTexture("../assets/images/background.jpg");
  Texture2D circle = LoadTexture("../assets/images/circle.png");
  Texture2D login = LoadTexture("../assets/images/login.png");
  Texture2D user = LoadTexture("../assets/images/user.png");
  Texture2D email = LoadTexture("../assets/images/email.png");
  Texture2D lock = LoadTexture("../assets/images/lock.png");
  Texture2D eye = LoadTexture("../assets/images/eyes.png");
  // Load Custom Fonts Poppins
  Font font = LoadFontEx("D:/Anonymous/Developer/MyRaylib/MineSweeper/hello_raylib/assets/fonts/Poppins/Poppins/Poppins-SemiBold.ttf", 48, 0, 250);

  // Input Boxes Position And Dimensions
  Rectangle full_name = {40.0f, 360.0f, (float)(screenWidth - 80), 70.0f};
  Rectangle email_address = {40.0f, 490.0f, (float)(screenWidth - 80), 70.0f};
  Rectangle password = {40.0f, 610.0f, (float)(screenWidth - 80), 70.0f};

  // Postion For InputBox Label
  Vector2 fullNameLabelPos = {50.0f, 335.0f};
  Vector2 emailLabelPos = {50.0f, 465.0f};
  Vector2 passwordLabelPos = {50.0f, 585.0f};
  // Position For Error Message
  Vector2 fullNameErrorPos = {50.0f, full_name.y + full_name.height + 5.0f};
  Vector2 emailErrorPos = {50.0f, email_address.y + email_address.height + 5.0f};
  Vector2 passwordErrorPos = {50.0f, password.y + password.height + 5.0f};
  // Initialize Boolean To Check The Current Active State Of Input Field
  bool fullNameFocused = false, emailFocused = false, passwordFocused = false;
  static bool passwordVisible = false;
  bool showResponse = false;
  double responseStartTime = 0;
  char responseMessage[128] = "";

  // Initialize The Variables To Store Users Input
  char fullName[128] = "";
  char emailText[128] = "";
  char passwordText[128] = "";
  int fullNameLen = 0, emailLen = 0, passwordLen = 0;
  // Initialize Variables For Error Message
  bool showError = false;
  char fullNameError[64] = "";
  char emailError[64] = "";
  char passwordError[64] = "";

  SetTargetFPS(90);
  // Run The Game Loop
  while (!WindowShouldClose())
  {
    // Get The Current Mouse Position
    Vector2 mouse = GetMousePosition();
    // Detect The Left Mouse Click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
      // Check If The Mouse Click Lies Inside The Particular Input Field Or Not
      fullNameFocused = CheckCollisionPointRec(mouse, full_name);
      emailFocused = CheckCollisionPointRec(mouse, email_address);
      passwordFocused = CheckCollisionPointRec(mouse, password);
      // Set Other Value Fasle Except The Current Input Field
      if (fullNameFocused)
      {
        emailFocused = passwordFocused = false;
      }
      else if (emailFocused)
      {
        fullNameFocused = passwordFocused = false;
      }
      else if (passwordFocused)
      {
        fullNameFocused = emailFocused = false;
      }
    }
    // Detect The Key Press From The User And Store Int ASCII Value
    int key = GetCharPressed();
    // Check If The Inputted Character Is Printable Or Not i.e Printable Character Lies Between 32 To 125
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
    // Delete The Previous Character If User Input Backspace
    if (IsKeyPressed(KEY_BACKSPACE))
    {
      if (fullNameFocused && fullNameLen > 0)
        fullName[--fullNameLen] = '\0';
      else if (emailFocused && emailLen > 0)
        emailText[--emailLen] = '\0';
      else if (passwordFocused && passwordLen > 0)
        passwordText[--passwordLen] = '\0';
    }
    // Button Click Event
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
      Rectangle create_account = {40.0f, 745.0f, (float)(screenWidth - 80), 65.0f};
      if (CheckCollisionPointRec(mouse, create_account))
      {
        showError = true;

        if (strlen(fullName) < 3)
          strcpy(fullNameError, "Enter at least 3 characters.");
        else
          strcpy(fullNameError, "");

        if (!IsValidEmail(emailText))
          strcpy(emailError, "Invalid email format.");
        else
          strcpy(emailError, "");

        if (!IsValidPassword(passwordText))
          strcpy(passwordError, "Password must have uppercase, lowercase, number.");
        else
          strcpy(passwordError, "");

        // Connection With Backend
        if (strlen(fullNameError) == 0 && strlen(emailError) == 0 && strlen(passwordError) == 0)
        {
          SignUp add(fullName, emailText, passwordText);
          std::string response = add.sendRequest();
          std::cout << response << std::endl;

          strncpy(responseMessage, response.c_str(), sizeof(responseMessage) - 1);
          responseMessage[sizeof(responseMessage) - 1] = '\0';

          showResponse = true;
          responseStartTime = GetTime(); // Store current time
        }
      }
    }

    // Initialize UI Rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Background Full Screen Rectangle
    Rectangle backgroundSrc = {0.0f, 0.0f, (float)background.width, (float)background.height};
    Rectangle backgroundDest = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};
    Vector2 zeroVec = {0.0f, 0.0f};
    DrawTexturePro(background, backgroundSrc, backgroundDest, zeroVec, 0.0f, WHITE);

    // Circle Texture
    Vector2 circlePos = {430.0f, -250.0f};
    DrawTexture(circle, (int)circlePos.x, (int)circlePos.y, WHITE);

    // Login Texture
    Vector2 loginPos = {170.0f, 10.0f};
    DrawTextureEx(login, loginPos, 0.0f, 0.6f, WHITE);

    // Rounded Rectangle Container
    Rectangle formRect = {0.0f, 250.0f, (float)screenWidth, (float)(screenHeight - 100)};
    DrawRectangleRounded(formRect, 0.1f, 20, WHITE);

    Vector2 createAccountTitlePos = {230.0f, 255.0f};
    DrawTextEx(font, "Create Account", createAccountTitlePos, 40.0f, 2.0f, BLUE);

    // --- Full Name Field ---
    DrawTextEx(font, "Full Name", fullNameLabelPos, 25.0f, 2.0f, BLACK);

    // Full Name Box Outlined If Focused
    if (fullNameFocused)
    {
      Rectangle fullNameOutline = {full_name.x - 2.0f, full_name.y - 2.0f, full_name.width + 4.0f, full_name.height + 4.0f};
      DrawRectangleRounded(fullNameOutline, 0.4f, 20, BLUE);
    }
    // Draw Full Name Input Field
    DrawRectangleRounded(full_name, 0.4f, 20, Color{216, 240, 251, 204});
    // Draw User Icon
    Vector2 userIconPos = {50.0f, 370.0f};
    DrawTextureEx(user, userIconPos, 0.0f, 0.09f, WHITE);
    // Draw Full Name Text
    Vector2 fullNameTextPos = {130.0f, 380.0f};
    DrawTextEx(font, fullNameLen > 0 ? fullName : "Full Name", fullNameTextPos, 25.0f, 2.0f, fullNameLen > 0 ? BLACK : GRAY);
    // Display Error Message
    if (showError && strlen(fullNameError) > 0)
    {
      DrawTextEx(font, fullNameError, fullNameErrorPos, 18.0f, 1.0f, RED);
    }

    // --- Email Field ---
    DrawTextEx(font, "Email Address", emailLabelPos, 25.0f, 2.0f, BLACK);
    // Check If Email Is Focused Or Not
    if (emailFocused)
    {
      Rectangle emailOutline = {email_address.x - 2.0f, email_address.y - 2.0f, email_address.width + 4.0f, email_address.height + 4.0f};
      DrawRectangleRounded(emailOutline, 0.4f, 20, BLUE);
    }
    // Draw Email Field
    DrawRectangleRounded(email_address, 0.4f, 20, Color{216, 240, 251, 204});
    // Draw Email Icon
    Vector2 emailIconPos = {50.0f, 500.0f};
    DrawTextureEx(email, emailIconPos, 0.0f, 0.13f, WHITE);
    // Draw Email Text
    Vector2 emailTextPos = {130.0f, 510.0f};
    DrawTextEx(font, emailLen > 0 ? emailText : "Email Address", emailTextPos, 25.0f, 2.0f, emailLen > 0 ? BLACK : GRAY);
    // Display Error Message
    if (showError && strlen(emailError) > 0)
    {
      DrawTextEx(font, emailError, emailErrorPos, 18.0f, 1.0f, RED);
    }

    // Add this variable outside the loop, e.g. near your other bools:

    // Inside your main loop, update this password field code as follows:

    // --- Password Field ---
    DrawTextEx(font, "Password", passwordLabelPos, 25.0f, 2.0f, BLACK);

    // Check If Password Field Is Focused Or Not
    if (passwordFocused)
    {
      Rectangle passwordOutline = {password.x - 2.0f, password.y - 2.0f, password.width + 4.0f, password.height + 4.0f};
      DrawRectangleRounded(passwordOutline, 0.4f, 20, BLUE);
    }

    // Draw Password Field
    DrawRectangleRounded(password, 0.4f, 20, Color{216, 240, 251, 204});

    // Draw Lock Icon
    Vector2 lockPos = {50.0f, 620.0f};
    DrawTextureEx(lock, lockPos, 0.0f, 0.099f, WHITE);

    // Draw Eye Icon
    Vector2 eyePos = {570.0f, 620.0f};
    DrawTextureEx(eye, eyePos, 0.0f, 0.099f, BLACK);

    // Detect Mouse Click On Eye Icon To Toggle Password Visibility
    Rectangle eyeButton = {eyePos.x, eyePos.y, 32.0f, 32.0f};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, eyeButton))
    {
      passwordVisible = !passwordVisible;
    }

    // Draw Small Slant line (slash) Over Eye Icon When Password Is Hidden
    if (!passwordVisible)
    {
      // Diagonal Length
      float lineLength = 23.0f;
      Vector2 lineStart = {eyePos.x + 36, eyePos.y + 13};
      // 45 Degree Down - Left
      Vector2 lineEnd = {lineStart.x - lineLength, lineStart.y + lineLength};
      DrawLineEx(lineStart, lineEnd, 3.0f, BLACK);
    }

    // Show Password Or Hidden Asterisks
    char hidden[128] = "";
    if (passwordLen > 0)
    {
      if (passwordVisible)
      {
        DrawTextEx(font, passwordText, Vector2{130.0f, 630.0f}, 25.0f, 2.0f, BLACK);
      }
      else
      {
        for (int i = 0; i < passwordLen; i++)
          hidden[i] = '*';
        hidden[passwordLen] = '\0';
        DrawTextEx(font, hidden, Vector2{130.0f, 630.0f}, 25.0f, 2.0f, BLACK);
      }
    }
    else
    {
      DrawTextEx(font, "Password", Vector2{130.0f, 630.0f}, 25.0f, 2.0f, GRAY);
    }

    // Display Password Error Message
    if (showError && strlen(passwordError) > 0)
    {
      DrawTextEx(font, passwordError, passwordErrorPos, 18.0f, 1.0f, RED);
    }

    // --- Create Account Button ---
    Rectangle create_account = {40.0f, 745.0f, (float)(screenWidth - 80), 65.0f};
    DrawRectangleRounded(create_account, 0.4f, 20, DARKBLUE);

    Vector2 createAccountBtnTextPos = {260.0f, 760.0f};
    DrawTextEx(font, "Create Account", createAccountBtnTextPos, 30.0f, 1.0f, WHITE);

    // --- Footer Texts ---
    Vector2 orTextPos = {340.0f, 850.0f};
    DrawTextEx(font, "OR", orTextPos, 30.0f, 1.0f, BLACK);

    Vector2 alreadyHaveAccountPos = {210.0f, 900.0f};
    DrawTextEx(font, "Already Have An Account ?", alreadyHaveAccountPos, 20.0f, 1.0f, BLACK);

    Vector2 signUpPos = {430.0f, 900.0f};
    DrawTextEx(font, "Sign Up", signUpPos, 20.0f, 1.0f, DARKPURPLE);
    // Toast To Display The Response Message

    if (showResponse && GetTime() - responseStartTime < 3.0)
    {
      DrawRectangleRounded(Rectangle{140, 835, 400, 50}, 0.6f, 10, Color{39, 120, 245, 255});

      float textWidth = MeasureTextEx(font, responseMessage, 25, 1.0f).x;
      
      float x = 150 + (400 - textWidth) / 2;
      if (strcmp(responseMessage, "User Already Exist") == 0)
        x += 10;

      Vector2 textPos = {x, 850};
      DrawTextEx(font, responseMessage, textPos, 25, 1.0f, WHITE);
    }
    else
    {
      showResponse = false;
    }

    EndDrawing();
  }
  // Free All The Memory Utilized
  UnloadTexture(background);
  UnloadTexture(circle);
  UnloadTexture(login);
  UnloadTexture(user);
  UnloadTexture(email);
  UnloadTexture(lock);
  UnloadTexture(eye);
  UnloadFont(font);
  CloseWindow();
  return 0;
}
