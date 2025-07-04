#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "raylib.h"

class SplashScreen {
private:
    Texture2D logoImage;
    Font nameFont;
    float startTime;
    float duration;
    bool finished;

public:
    SplashScreen(float splashDuration = 3.0f);
    ~SplashScreen();

    void Update();
    void Draw();
    bool IsFinished() const;
};

#endif
