#include "SplashScreen.h"

SplashScreen::SplashScreen(float splashDuration)
    : duration(splashDuration), finished(false)
{
    logoImage = LoadTexture("../assets/images/login.png");
    nameFont = LoadFontEx("D:/Anonymous/Developer/MyRaylib/MineSweeper/hello_raylib/assets/fonts/Poppins/Poppins/Poppins-SemiBold.ttf", 48, 0, 250);
    startTime = GetTime();
}

SplashScreen::~SplashScreen() {
    UnloadTexture(logoImage);
    UnloadFont(nameFont);
}

void SplashScreen::Update() {
    float elapsed = GetTime() - startTime;
    if (elapsed >= duration) {
        finished = true;
    }
}

void SplashScreen::Draw() {
    float elapsed = GetTime() - startTime;
    const float fadeDuration = 3.0f;

    // Fade-in only (no fade-out)
    float alpha = (elapsed < fadeDuration) ? (elapsed / fadeDuration) : 1.0f;

    // Scale grows from 0.8 to 1.0
    float scale = 0.8f + (0.2f * alpha);

    // Text size grows from 30 to 40
    float fontSize = 30.0f + 10.0f * alpha;

    Color fadeColor = {255, 255, 255, static_cast<unsigned char>(alpha * 255)};
    Color textColor = {0, 0, 0, static_cast<unsigned char>(alpha * 255)};

    ClearBackground(RAYWHITE);

    // Scaled image centered
    float imgWidth = logoImage.width * scale;
    float imgHeight = logoImage.height * scale;
    float imgX = (GetScreenWidth() - imgWidth) / 2.0f;
    float imgY = (GetScreenHeight() - imgHeight) / 2.0f - 50;

    DrawTexturePro(
        logoImage,
        { 0, 0, static_cast<float>(logoImage.width), static_cast<float>(logoImage.height) },
        { imgX, imgY, imgWidth, imgHeight },
        { 0, 0 },
        0.0f,
        fadeColor
    );

    // Text centered below image
    const char* text = "MineSweeper";
    Vector2 textSize = MeasureTextEx(nameFont, text, fontSize, 2);
    float textX = (GetScreenWidth() - textSize.x) / 2.0f;
    float textY = imgY + imgHeight + 20;

    DrawTextEx(nameFont, text, {textX, textY}, fontSize, 2, textColor);
}

bool SplashScreen::IsFinished() const {
    return finished;
}
