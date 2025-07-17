#include "home_screen.h"
#include <raylib.h>
#include<user_profile.h>
#include<iostream>
#include<string>
#include<leaderboard.h>

// -------- Button class --------
Texture2D newGameIcon;
int Clamp(int value, int min, int max)
{
    return (value < min) ? min : (value > max) ? max : value;
}

float Clampf(float value, float min, float max)
{
    return (value < min) ? min : (value > max) ? max : value;
}

// -------- Button class --------

Button::Button(float x, float y, float w, float h, const std::string& label, const std::string& iconPath)
    : rect({x, y, w, h}), text(label), hovered(false)
{
    baseColor = {40, 50, 70, 255};
    hoverColor = {70, 100, 150, 255};
    textColor = RAYWHITE;
    icon = LoadTexture(iconPath.c_str());
}

Button::Button(float x, float y, float w, float h, const std::string& label)
    : rect({x, y, w, h}), text(label), hovered(false)
{
    baseColor = {40, 50, 70, 255};
    hoverColor = {70, 100, 150, 255};
    textColor = RAYWHITE;
    icon.id = 0;  // Invalid icon (none)
}





// Helper function to darken a color by a given factor (0.0 to 1.0)
Color DarkenColor(Color c, float factor)
{
    factor = Clamp(factor, 0.0f, 1.0f); // Ensure the factor is between 0 and 1

    return {
        (unsigned char)Clamp((int)(c.r * factor), 0, 255),
        (unsigned char)Clamp((int)(c.g * factor), 0, 255),
        (unsigned char)Clamp((int)(c.b * factor), 0, 255),
        c.a // Preserve alpha
    };
}


void Button::Draw(Font *font)
{
    float roundness = 0.25f;
    int segments = 16;

    DrawRectangleRounded({rect.x, rect.y + 6, rect.width, rect.height}, roundness, segments, Fade(GRAY, 0.25f));

    Rectangle drawRect = {rect.x, rect.y, rect.width, rect.height};
    Color colorToDraw = hovered ? hoverColor : baseColor;

    Color borderColor = DarkenColor(baseColor, 0.65f);
    float borderThickness = 4.0f;
    for (float t = 0; t < borderThickness; t += 1.0f) {
        Rectangle borderRect = {
            drawRect.x - t / 2,
            drawRect.y - t / 2,
            drawRect.width + t,
            drawRect.height + t
        };
    }

    DrawRectangleRounded(drawRect, roundness, segments, colorToDraw);

    float iconSize = rect.height * 0.4f;
    Rectangle iconRect = {
        drawRect.x + (drawRect.width - iconSize) / 2,
        drawRect.y + 10,
        iconSize, iconSize
    };

    if (icon.id != 0) {
        DrawTexturePro(
            icon,
            {0, 0, (float)icon.width, (float)icon.height},
            iconRect,
            {0, -30},
            1.0f,
            WHITE
        );
    }

    int fontSize = (int)(rect.height * 0.20f);
    float spacing = 1.0f;
    Vector2 textSize = MeasureTextEx(*font, text.c_str(), fontSize, spacing);

    Vector2 textPos = {
        drawRect.x + (drawRect.width - textSize.x) / 2,
        iconRect.y + iconRect.height + 50
    };

    DrawTextEx(*font, text.c_str(), {textPos.x + 1, textPos.y +4 }, fontSize, spacing, BLUE);
    DrawTextEx(*font, text.c_str(), textPos, fontSize, spacing, RAYWHITE);
}




// -------- UIManager class --------
bool Button::Update(Vector2 mousePos, bool mousePressed)
{
    hovered = CheckCollisionPointRec(mousePos, rect);
    return hovered && mousePressed;
}


void UIManager::Init()
{
    customFont = LoadFontEx("D:/Anonymous/Developer/MyRaylib/MineSweeper/hello_raylib/assets/fonts/Poppins/Poppins/Poppins-Bold.ttf", 48, 0, 250);
 background = LoadTexture("../assets/images/background.jpg");
  mineBg= LoadTexture("../assets/images/mineBg.png");
  corner = LoadTexture("../assets/images/corner.png");
   New_Game = LoadTexture("../assets/images/New_Game.png");
   lines=LoadTexture("../assets/images/3lines.png");
      notification=LoadTexture("../assets/images/notification.png");
            user=LoadTexture("../assets/images/user.png");
              border=LoadTexture("../assets/images/border.png");

    corner_bottom = LoadTexture("../assets/images/corner_bottom.png");  // Load background into class member

    homeIcon =LoadTexture("../assets/images/game.png");

    // Home buttons
// Sample colors for different buttons
// Define bold, vibrant colors for home buttons (no heavy fade on hover)
// Example bold deep colors defined here or globally
Color boldDeepColors[] = {
    {0, 142, 252,   255}, // Bright Red-Orange (opacity 0.8)
    {56, 25, 244, 255}, // Neon Green
    {4, 143, 253,  255}, // Cyan Blue
    {130, 4, 253,  255}, // Purple
    {25, 48, 128,  255}, // Pink
    {99, 4, 253,   255}  // Violet
};



// Home buttons
float btnW = 200, btnH = 200, btnX = 290;
float startY = 340, gap = 24;
homeButtons.clear();

std::vector<std::string> homeLabels = {
    "New Game", "Sound", "Global", "Rules", "Exit","Account"
};

int cols = 3; // Number of columns in the grid
float btnWidth = 200, btnHeight = 200;
float startX = 55;
float startsY = 480;
float gapX = 50;  // Horizontal gap between buttons
float gapY = 50;  // Vertical gap between buttons


homeButtons.clear();
std::vector<std::string> homeIconPaths = {
    "../assets/images/start.png",
    "../assets/images/sound.png",
    "../assets/images/leaderboard.png",
    "../assets/images/rules.png",
    "../assets/images/close.png",
    "../assets/images/signUp.png"
};


for (int i = 0; i < (int)homeLabels.size(); ++i) {
    int col = i % cols;
    int row = i / cols;

    float x = startX + col * (btnWidth + gapX);
    float y = startsY + row * (btnHeight + gapY);

   Button btn(x, y, btnWidth, btnHeight, homeLabels[i],homeIconPaths[i]);

    btn.baseColor = boldDeepColors[i];
    btn.hoverColor = Fade(boldDeepColors[i], 0.6f);
    homeButtons.push_back(btn);
}



// Options buttons
optionsButtons.clear();
optionsButtons.emplace_back(300, 240, 380, 60, "Toggle Sound");
optionsButtons.back().baseColor = {44, 62, 80, 255};
optionsButtons.back().hoverColor = Fade({44, 62, 80, 255}, 0.95f);

optionsButtons.emplace_back(300, 320, 380, 60, "Toggle Music");
optionsButtons.back().baseColor = {41, 128, 185, 255};
optionsButtons.back().hoverColor = Fade({41, 128, 185, 255}, 0.95f);

optionsButtons.emplace_back(300, 400, 380, 60, "Back");
optionsButtons.back().baseColor = {192, 57, 43, 255};
optionsButtons.back().hoverColor = Fade({192, 57, 43, 255}, 0.95f);


// Level selection buttons: bold colors with subtle hover highlight
levelButtons.clear();

struct LevelButton {
    const char* label;
    Color color;
    const char* iconPath;
} levelData[] = {
    {"Easy (12x12)", {39, 174, 96, 255}, "../assets/images/easy_icon.png"},
    {"Medium (15x15)", {243, 156, 18, 255}, "../assets/images/medium_icon.png"},
    {"Hard (20x20)", {192, 57, 43, 255}, "../assets/images/hard_icon.png"}
};


for (int i = 0; i < 3; ++i) {
    Button btn(btnX, 220 + i * 80, btnW, btnH, levelData[i].label, levelData[i].iconPath);
    btn.baseColor = levelData[i].color;
    btn.hoverColor = Fade(levelData[i].color, 0.95f);
    levelButtons.push_back(btn);
}



}

void UIManager::Unload()
{
    if (customFont.texture.id != 0)
        UnloadFont(customFont);
    if (homeIcon.id != 0)
        UnloadTexture(homeIcon);
    if (background.id != 0)
        UnloadTexture(background);
    if (corner.id != 0)
        UnloadTexture(corner);
    if (corner_bottom.id != 0)
        UnloadTexture(corner_bottom);
    if (New_Game.id != 0)
        UnloadTexture(New_Game);

    // Unload icons from home buttons
    for (auto& btn : homeButtons) {
        if (btn.icon.id != 0)
            UnloadTexture(btn.icon);
    }

    for (auto& btn : levelButtons) {
        if (btn.icon.id != 0)
            UnloadTexture(btn.icon);
    }
}


int UIManager::UpdateHome(Vector2 mousePos, bool mousePressed)
{
    for (int i = 0; i < (int)homeButtons.size(); i++)
    {
        if (homeButtons[i].Update(mousePos, mousePressed))
        {
            if (i == 4) // Exit button
                CloseWindow();
            return i;
        }
    }
    return -1;
}

void UIManager::DrawHome()

{
      UserProfile profile("siddhanttimalsina10@gmail.com");
    UserProfileData data = profile.fetchProfile();
  Color myColor = {218, 241, 240, (unsigned char)(0.8f * 255)}; // => alpha = 204

    if (background.id != 0)
        DrawTexturePro(background,
                       Rectangle{0, 0, (float)background.width, (float)background.height},
                       Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                       Vector2{0, 0}, 0.0f, myColor);

   Rectangle roundedRect = {
            0,                     // x position (start at left)
          460, // y position (so it's vertically centered)
            (float)800,   // width (full screen)
            970 / 1.5f   // height (half of screen)
        };

        float roundness = 0.2f;   // Adjust corner roundness (0.0 to 1.0)
        int segments = 30;        // Smoothness of rounded corners
Color softWhiteCyan = {232, 243, 243, 204};


        DrawRectangleRounded(roundedRect, roundness, segments, WHITE);

    // ✅ Option 1: Just remove this (not needed anymore)
    // ClearBackground({22, 28, 40, 255});

    // ✅ Option 2: Only use it if background fails to load
     //  DrawTexture(corner, 0, 0, WHITE);
  // Example: Draw a texture at (200, 200) with custom width and height
float customWidth = 450;
float customHeight = 150;
// Left rectangle parameters
int rectXLeft = 10;
int rectWidth = 80;
int rectHeight = 80;
int rectY = 20;

// Draw left rounded rectangle
Rectangle rectLeft = { (float)rectXLeft, (float)rectY, (float)rectWidth, (float)rectHeight };
Color customColor = { 121, 164, 231, 204 };  // RGBA with 0.8 alpha
DrawRectangleRounded(rectLeft, 0.2f, 12, customColor);

// Resize and center the image texture inside the left rectangle
float iconTargetWidth = rectWidth * 0.8f;
float iconTargetHeight = rectHeight * 0.8f;

Rectangle srcRec = { 0.0f, 0.0f, (float)lines.width, (float)lines.height };
Rectangle destRecLeft = {
    rectXLeft + (rectWidth - iconTargetWidth) / 2.0f,
    rectY + (rectHeight - iconTargetHeight) / 2.0f,
    iconTargetWidth,
    iconTargetHeight
};
Vector2 origin = { 0.0f, 0.0f };
DrawTexturePro(lines, srcRec, destRecLeft, origin, 0.0f, WHITE);

// --- Now draw the same on the right side ---

// Calculate right rectangle X position with 10 px padding from right edge
int screenWidth = GetScreenWidth();  // get your window width dynamically
int rectXRight = screenWidth - rectWidth - 10;

// Draw right rounded rectangle
Rectangle rectRight = { (float)rectXRight, (float)rectY, (float)rectWidth, (float)rectHeight };
DrawRectangleRounded(rectRight, 0.2f, 12, customColor);

// Center the same image inside right rectangle
Rectangle destRecRight = {
    rectXRight + (rectWidth - iconTargetWidth+5),
    rectY + (rectHeight - iconTargetHeight+5),
 rectWidth * 0.5f,
   rectHeight * 0.5f
};
DrawTexturePro(notification, srcRec, destRecRight, origin, 0.0f, WHITE);




DrawTexturePro(
    mineBg,                                      // Texture to draw
    Rectangle{0, 0, (float)mineBg.width, (float)mineBg.height},  // Source rectangle (full texture)
    Rectangle{275,-15, customWidth, customHeight},              // Destination rectangle (position + custom size)
    Vector2{100, 0},                               // Origin (top-left corner)
    0.0f,                                        // Rotation (no rotation)
    WHITE                                        // Tint color
);

        DrawTexture(corner_bottom,
            GetScreenWidth() - corner_bottom.width,
            GetScreenHeight() - corner_bottom.height,
            WHITE);
    if (background.id == 0)
        ClearBackground({22, 28, 40, 255});
        //Code Starts Here

const char* title = "MINESWEEPER";
int fontSize = 72;
float spacing = 2.0f;
 //DrawTexture(New_Game, -300,800, WHITE);

// Measure total title size for centering
Vector2 totalSize = {0};
for (int i = 0; i < TextLength(title); i++) {
    const char* ch = TextSubtext(title, i, 1);
    Vector2 size = MeasureTextEx(customFont, ch, fontSize, spacing);
    totalSize.x += size.x + spacing;
    if (size.y > totalSize.y) totalSize.y = size.y;
}
totalSize.x -= spacing;

// Calculate centered position
Vector2 titlePos = {
    (GetScreenWidth() - totalSize.x) / 2,
    25
};

// Gradient color palette
Color gradientColors[] = {
    RED, ORANGE, YELLOW, GREEN, SKYBLUE, BLUE, PURPLE, PINK, GOLD, LIME
};
int gradLen = sizeof(gradientColors) / sizeof(Color);

// Draw each character with gradient and shadow
Vector2 charPos = titlePos;
for (int i = 0; i < TextLength(title); i++) {
    const char* ch = TextSubtext(title, i, 1);
    Color col = gradientColors[i % gradLen];

    Vector2 chSize = MeasureTextEx(customFont, ch, fontSize, spacing);

    // Shadow
    DrawTextEx(customFont, ch, {charPos.x + 3, charPos.y + 3}, fontSize, spacing, BLACK);

    // Colored letter
    DrawTextEx(customFont, ch, charPos, fontSize, spacing, col);


    charPos.x += chSize.x + spacing;
}





//DrawTextEx(customFont, "CHOOSE", {305, 320}, 64, 1, RED);



    float iconW = 250, iconH = 250;
    float iconX = 400 - iconW / 2;
    float iconY = 120;
    //if (homeIcon.id != 0)
        //DrawTexturePro(homeIcon, {0, 0, (float)homeIcon.width, (float)homeIcon.height},
                     //  {iconX, iconY, iconW, iconH}, {0, 0}, 0, WHITE);


                     //User Profile Section
            screenWidth = GetScreenWidth();
int screenHeight = GetScreenHeight();

// Outer rectangle dimensions
int marginX = 20;
int rectX = marginX;
 rectY = 130;
 rectWidth = screenWidth - 2 * marginX;
 rectHeight = screenHeight / 3.0;

Rectangle fullWidthRect = { (float)rectX, (float)rectY, (float)rectWidth, (float)rectHeight };
Color bgColor = { 170, 234, 243, 204 };  // rgba(170, 234, 243, 0.8)

// Draw outer rounded rectangle
DrawRectangleRounded(fullWidthRect, 0.2f, 12, bgColor);

// Inner rectangle settings
int innerMargin = 10;
int innerWidth = (rectWidth - 3 * innerMargin) / 2;  // Space for 2 rectangles + margins
int innerHeight = rectHeight - 2 * innerMargin;
int innerY = rectY + innerMargin;

// Left inner rectangle
int innerLeftX = rectX + innerMargin;
Rectangle leftRect = {
    (float)innerLeftX,
    (float)innerY,
    (float)innerWidth,
    (float)innerHeight
};

// Right inner rectangle
int innerRightX = innerLeftX + innerWidth + innerMargin;
Rectangle rightRect = {
    (float)innerRightX,
    (float)innerY,
    (float)innerWidth,
    (float)innerHeight
};

// Draw inner rectangles
Color innerColor = WHITE;
DrawRectangleRounded(leftRect, 0.15f, 10, innerColor);
// Circle avatar settings
float avatarRadius = leftRect.width * 0.3f;  // 30% of card width
float avatarCenterX = leftRect.x + leftRect.width / 2.0f;
float avatarCenterY = leftRect.y + avatarRadius + 10;  // 10px margin from top

// Draw background circle (to simulate border/mask)

// Calculate scaled image size (crop square)
float imageSize = avatarRadius * 1.3f;
Rectangle srcRecs = { 0.0f, 0.0f, (float)user.width, (float)user.height };
//Here is Your comment
Rectangle srcRecsRight = { 0.0f, 0.0f, (float)user.width, (float)user.height };
Rectangle destRec = {
    avatarCenterX - imageSize / 2.0f,
    avatarCenterY - imageSize / 1.25f,
    imageSize,
    imageSize
};
Vector2 origins = { 0.0f, 0.0f };
// Draw user name below the avatar
DrawTextEx(customFont, data.user_name.c_str(),
    Vector2{
        avatarCenterX - MeasureTextEx(customFont, data.user_name.c_str(), 29, 1).x / 2.0f,
        avatarCenterY + 40
    },
    30, 1, BLACK);

DrawTextEx(customFont, "Best ",
    Vector2{70, avatarCenterY + 100},
    22, 1, BLACK);

Rectangle best = {50, 390, 80, 40};
Color pinkRedColor = {255, 0, 100, 255};
DrawRectangleRounded(best, 0.3f, 12, pinkRedColor);
std::string bestTimeFormatted = std::to_string(std::stoi(data.bestTime) / 60) + ":" + (std::stoi(data.bestTime) % 60 < 10 ? "0" : "") + std::to_string(std::stoi(data.bestTime) % 60);


DrawTextEx(customFont, bestTimeFormatted.c_str(),
    Vector2{73, avatarCenterY + 140},
    25, 1, WHITE);

DrawTextEx(customFont, "Wins",
    Vector2{192, avatarCenterY + 100},
    25, 1, BLACK);

Rectangle win = {177, 390, 80, 40};
Color blueColor = {0, 95, 255, 255};
DrawRectangleRounded(win, 0.3f, 12, blueColor);

DrawTextEx(customFont, data.won.c_str(),
    Vector2{210, avatarCenterY + 140},
    25, 1, WHITE);

DrawTextEx(customFont, "Lose",
    Vector2{310, avatarCenterY + 100},
    25, 1, BLACK);

Rectangle lose = {295, 390, 80, 40};
Color violetColor = {139, 0, 255, 255};
DrawRectangleRounded(lose, 0.3f, 12, violetColor);

DrawTextEx(customFont, data.lose.c_str(),
    Vector2{330, avatarCenterY + 140},
    25, 1, WHITE);



// Draw the user profile texture (simulated inside circle)
DrawTexturePro(user, srcRecs, destRec, origins, 0.0f, WHITE);
// Define source rectangle (entire texture)





//DrawRectangleRounded(rightRect, 0.15f, 10, innerColor);

// Static/global variables (define once in your scope)
// Static/global variables (define once in your scope)
static std::vector<Color> rectColors = {
    {255, 137, 198, 204},  // rgba(255, 137, 198, 0.8)
    {191, 137, 255, 204},  // rgba(191, 137, 255, 0.8)
    {137, 213, 255, 204},  // rgba(137, 213, 255, 0.8)
    {255, 230, 137, 204},  // rgba(255, 230, 137, 0.8)
    {137, 145, 255, 204}   // rgba(137, 145, 255, 0.8)
};



static std::vector<const char*> path= {"../assets/images/1st.png", "../assets/images/2nd.png", "../assets/images/3rd.png", "../assets/images/4th.png", "../assets/images/5th.png"};

static std::vector<std::string> names;
static std::vector<std::string> bestTimes;
static std::vector<int> wins;
static std::vector<int> losses;

if (names.empty()) {
    auto players = LeaderBoard::fetchTopPlayers("http://localhost:18080/playersInfo");

  for (const auto& p : players) {
    names.push_back(p.user_name);

    int totalSeconds = p.bestTime; // already int
    int mins = totalSeconds / 60;
    int secs = totalSeconds % 60;
    bestTimes.push_back(TextFormat("%02d:%02d", mins, secs));

    wins.push_back(p.won);   // already int
    losses.push_back(p.lose); // already int
}


}


static int totalRects = (int)rectColors.size();
static int currentRectIndex = 0;
static int nextRectIndex = 1;

static float rectTimer = 0.0f;
static float rectSwitchInterval = 3.0f;

static bool rectIsFading = false;
static float rectFadeTimer = 0.0f;
static float rectFadeDuration = 0.6f;

// Load image once (example path)
 avatar = LoadTexture(path[currentRectIndex]);  // Replace with your actual image path

// In your update/draw loop:
float dt = GetFrameTime();
rectTimer += dt;

if (!rectIsFading && rectTimer >= rectSwitchInterval) {
    rectTimer = 0.0f;
    rectIsFading = true;
    rectFadeTimer = 0.0f;
    nextRectIndex = (currentRectIndex + 1) % totalRects;
}

if (rectIsFading) {
    rectFadeTimer += dt;
    if (rectFadeTimer >= rectFadeDuration) {
        rectIsFading = false;
        currentRectIndex = nextRectIndex;
        rectFadeTimer = 0.0f;
    }
}

float alphaCurrent = 1.0f;
float alphaNext = 0.0f;

if (rectIsFading) {
    float t = rectFadeTimer / rectFadeDuration;
    alphaCurrent = 1.0f - t;
    alphaNext = t;
}

// Draw the background of the current card
DrawRectangleRounded(rightRect, 0.15f, 10, Fade(rectColors[currentRectIndex], alphaCurrent));

// If fading, draw the next background on top
if (rectIsFading) {
    DrawRectangleRounded(rightRect, 0.15f, 10, Fade(rectColors[nextRectIndex], alphaNext));
}

// Avatar positioning
float avatarRadiusRight = rightRect.width * 0.3f;
float avatarCenterXRight = rightRect.x + rightRect.width / 2.0f;
float avatarCenterYRight = rightRect.y + avatarRadiusRight + 10;

float imageSizeRight = avatarRadiusRight * 1.3f;
Rectangle srcRecsRights = { 0.0f, 0.0f, (float)avatar.width, (float)avatar.height };
Rectangle destRecRights = {
    avatarCenterXRight - imageSizeRight / 2.0f,
    avatarCenterYRight - imageSizeRight / 1.25f,
    imageSizeRight,
    imageSizeRight
};
Vector2 originsRight = { 0.0f, 0.0f };

// Draw avatar image (simulated circle)
DrawTexturePro(avatar, srcRecsRights, destRecRights, originsRight, 0.0f, WHITE);

// Get profile data for current card
const char* name = names[currentRectIndex].c_str();
const char* bestTime = bestTimes[currentRectIndex].c_str();
int winning = wins[currentRectIndex];
int loses = losses[currentRectIndex];


// Draw username centered
DrawTextEx(customFont, name,
    Vector2{
        avatarCenterXRight - MeasureTextEx(customFont, name, 29, 1).x / 2.0f,
        avatarCenterYRight + 40
    },
    30, 1, BLACK);

// BEST TIME
DrawTextEx(customFont, "Best",
    Vector2{ rightRect.x + 40, avatarCenterYRight + 100 },
    25, 1, BLACK);

Rectangle bestRect = { rightRect.x + 25, avatarCenterYRight + 130, 80, 40 };
DrawRectangleRounded(bestRect, 0.3f, 12, pinkRedColor);

DrawTextEx(customFont, bestTime,
    Vector2{ bestRect.x + 15, bestRect.y + 8 },
    25, 1, WHITE);

// WINS
DrawTextEx(customFont, "Wins",
    Vector2{ rightRect.x + 155, avatarCenterYRight + 100 },
    25, 1, BLACK);

Rectangle winRect = { rightRect.x + 145, avatarCenterYRight + 130, 80, 40 };
DrawRectangleRounded(winRect, 0.3f, 12, blueColor);

DrawTextEx(customFont, TextFormat("%d", winning),
    Vector2{ winRect.x + 30, winRect.y + 8 },
    25, 1, WHITE);

// LOSE
DrawTextEx(customFont, "Lose",
    Vector2{ rightRect.x + 275, avatarCenterYRight + 100 },
    25, 1, BLACK);

Rectangle loseRect = { rightRect.x + 260, avatarCenterYRight + 130, 80, 40 };
DrawRectangleRounded(loseRect, 0.3f, 12, violetColor);

DrawTextEx(customFont, TextFormat("%d", loses),
    Vector2{ loseRect.x + 33, loseRect.y + 8 },
    25, 1, WHITE);

// Dot indicators
int dotRadius = 6;
int dotSpacing = 25;
int dotStartX = (int)(rightRect.x + rightRect.width / 2)+20 - (totalRects * dotSpacing) / 2;
int dotPosY = (int)(rightRect.y + rightRect.height) + 10;

for (int i = 0; i < totalRects; i++) {
    Color dotColor = (i == currentRectIndex) ? DARKGRAY : LIGHTGRAY;
    DrawCircle((float)(dotStartX + i * dotSpacing), (float)dotPosY, (float)dotRadius, dotColor);
}









    for (auto &btn : homeButtons)
        btn.Draw(&customFont);
}





int UIManager::UpdateOptions(Vector2 mousePos, bool mousePressed)
{
    for (int i = 0; i < (int)optionsButtons.size(); i++)
    {
        if (optionsButtons[i].Update(mousePos, mousePressed))
            return i;
    }
    return -1;
}

void UIManager::DrawOptions(bool soundOn, bool musicOn)
{
    ClearBackground({22, 28, 40, 255});
    DrawTextEx(customFont, "OPTIONS", {320, 120}, 64, 1, RAYWHITE);

    for (int i = 0; i < (int)optionsButtons.size(); i++)
    {
        Button &btn = optionsButtons[i];
        btn.Draw(&customFont);
        if (i == 0)
        {
            DrawText(soundOn ? "ON" : "OFF", (int)(btn.rect.x + btn.rect.width - 60), (int)(btn.rect.y + 15), 36, soundOn ? GREEN : RED);
        }
        else if (i == 1)
        {
            DrawText(musicOn ? "ON" : "OFF", (int)(btn.rect.x + btn.rect.width - 60), (int)(btn.rect.y + 15), 36, musicOn ? GREEN : RED);
        }
    }
}

int UIManager::UpdateLevelSelection(Vector2 mousePos, bool mousePressed)
{
    for (int i = 0; i < (int)levelButtons.size(); i++)
    {
        if (levelButtons[i].Update(mousePos, mousePressed))
            return i;
    }
    return -1;
}

void UIManager::DrawLevelSelection()
{
    ClearBackground({22, 28, 40, 255});
    DrawTextEx(customFont, "Select Difficulty", {270, 120}, 56, 1, RAYWHITE);
    for (auto &btn : levelButtons)
        btn.Draw(&customFont);
}

void UIManager::DrawHowToPlay()
{
    ClearBackground({220, 220, 220, 255});
    DrawTextEx(customFont, "HOW TO PLAY", {320, 80}, 64, 1, BLACK);

    Rectangle box = {100, 160, 700, 400};
    DrawRectangleRounded(box, 0.15f, 12, {30, 40, 60, 220});
    DrawRectangleRoundedLines(box, 0.15f, 12, BLACK);

    struct Instruction
    {
        const char *text;
        int icon; // 0 = bomb, 1 = flag, -1 = no icon
    };

    Instruction instructions[] = {
        {"Left Click: Reveal tile", 0},
        {"Right Click: Flag tile", 1},
        {"Reveal all safe tiles to win.", -1},
        {"Avoid the mines!", -1},
        {"", -1},
        {"Press ENTER to return to the main menu.", -1}};

    static Texture2D icons[2] = {
        LoadTexture("../assets/images/bomb.png"),
        LoadTexture("../assets/images/flag.png")};

    const float iconSize = 32.0f;
    const float iconPadding = 12.0f;
    int startX = (int)box.x + 40;
    int startY = (int)box.y + 40;
    int lineHeight = 50;

    for (int i = 0; i < (int)(sizeof(instructions) / sizeof(instructions[0])); i++)
    {
        int y = startY + i * lineHeight;

        if (instructions[i].icon >= 0)
        {
            float iconY = y + (lineHeight - iconSize) / 2.0f;
            Rectangle sourceRec = {0, 0, (float)icons[instructions[i].icon].width, (float)icons[instructions[i].icon].height};
            Rectangle destRec = {(float)startX, iconY, iconSize, iconSize};
            Vector2 origin = {0, 0};
            DrawTexturePro(icons[instructions[i].icon], sourceRec, destRec, origin, 0.0f, WHITE);

            float textX = startX + iconSize + iconPadding;
            DrawTextEx(customFont, instructions[i].text, {textX, (float)y}, 30, 1, BLACK);
        }
        else
        {
            DrawTextEx(customFont, instructions[i].text, {(float)startX, (float)y}, 30, 1, BLACK);
        }
    }

    // Note: UnloadTexture(icons[0]) and UnloadTexture(icons[1]) should be called in UIManager::Unload()
}
