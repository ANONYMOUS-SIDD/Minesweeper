#include "home_screen.h"
#include <raylib.h>
#include<user_profile.h>
#include<iostream>
#include<string>

#include<leaderboard.h>

// -------- Button class --------
std::string emails;

void UIManager::setEmails(std::string userEmail){
  emails=userEmail;
}


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

            // Add account screen navigation
            if (i == 5) // Account button
                return 100; // Special value for account screen

            return i;
        }
    }
    return -1;
}


void UIManager::DrawHome( )

{



      //UserProfile profile("siddhanttimalsina10@gmail.com");
       UserProfile profile(emails);
    UserProfileData data = profile.fetchProfile();
  Color myColor = {218, 241, 240, (unsigned char)(0.8f * 255)}; // => alpha = 204

    if (background.id != 0)
        DrawTexturePro(background,
                       Rectangle{0, 0, (float)background.width, (float)background.height},
                       Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                       Vector2{0, 0}, 0.0f, myColor);

   Rectangle roundedRect = {
            0,
          460,
            (float)800,
            970 / 1.5f
        };

        float roundness = 0.2f;
        int segments = 30;
Color softWhiteCyan = {232, 243, 243, 204};


  DrawRectangleRounded(roundedRect, roundness, segments, WHITE);

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









    float iconW = 250, iconH = 250;
    float iconX = 400 - iconW / 2;
    float iconY = 120;



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
void UIManager::DrawOptions(bool soundOn, bool musicOn) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    ClearBackground({22, 28, 40, 255}); // Dark blue-gray background

    // Main UI panel box
    int panelWidth = 600;
    int panelHeight = 500;
    Rectangle panelRect = {(float)(screenWidth - panelWidth) / 2, (float)(screenHeight - panelHeight) / 2, (float)panelWidth, (float)panelHeight};
    Color panelColor = {35, 45, 60, 230}; // Slightly lighter, semi-transparent panel background
    Color panelBorderColor = {60, 80, 100, 255}; // A subtle border color

    DrawRectangleRounded(panelRect, 0.5f, 10, panelColor);
    DrawRectangleRoundedLines(panelRect, 0.5f, 2, panelBorderColor);

    // Drawing a modern, glowing title inside the panel
    const char *titleText = "OPTIONS";
    int fontSize = 64;
    Vector2 textSize = MeasureTextEx(customFont, titleText, fontSize, 2);
    Vector2 titlePos = {panelRect.x + (panelWidth - textSize.x) / 2, panelRect.y + 50}; // Position title within the panel
    for (int i = 4; i >= 1; i--) {
        float alpha = 0.1f * i;
        DrawTextEx(customFont, titleText, titlePos, fontSize, 2, Fade(Color{100, 150, 255, 255}, alpha));
    }
    DrawTextEx(customFont, titleText, titlePos, fontSize, 2, RAYWHITE);

    // Modern UI layout variables for options
    int optionsY = panelRect.y + 150;
    int optionWidth = 400;
    int optionHeight = 60;
    int optionSpacing = 30;
    int optionX = panelRect.x + (panelWidth - optionWidth) / 2;

    // Drawing Sound and Music toggles (assuming optionsButtons[0] and optionsButtons[1])
    for (int i = 0; i < 2; i++) {
        Button &btn = optionsButtons[i];

        // Determine button position
        Rectangle rect = (i == 0) ? Rectangle{ (float)optionX, (float)optionsY, (float)optionWidth, (float)optionHeight } : Rectangle{ (float)optionX, (float)(optionsY + optionHeight + optionSpacing), (float)optionWidth, (float)optionHeight };

        // Draw the option name
        const char* labelText = (i == 0) ? "Sound" : "Music";
        Vector2 labelSize = MeasureTextEx(customFont, labelText, 36, 1);
        DrawTextEx(customFont, labelText, { rect.x + 20, rect.y + (rect.height - labelSize.y) / 2 }, 36, 1, RAYWHITE);

        // Drawing the toggle switch based on state
        bool state = (i == 0) ? soundOn : musicOn;
        Rectangle switchRect = { rect.x + rect.width - 120, rect.y + (rect.height - 50) / 2, 100, 50 };
        Color baseColor = state ? Color{40, 167, 69, 255} : Color{108, 117, 125, 255}; // Green for ON, Gray for OFF
        Color knobColor = RAYWHITE;
        float radius = 20;
        float knobX = state ? switchRect.x + switchRect.width - radius - 5 : switchRect.x + radius + 5;

        DrawRectangleRounded(switchRect, 0.7f, 8, baseColor);
        DrawRectangleRoundedLines(switchRect, 0.7f, 2, Fade(baseColor, 0.8f));
        DrawCircle((int)knobX, (int)(switchRect.y + switchRect.height / 2), radius, knobColor);

        // Position the button (for interaction) to match the toggle switch's rectangle
        btn.rect.x = switchRect.x;
        btn.rect.y = switchRect.y;
        btn.rect.width = switchRect.width;
        btn.rect.height = switchRect.height;
    }

    // Drawing and positioning the "Back" button (assuming optionsButtons[2])
    const char *backText = "Back";
    int backButtonWidth = 200;
    int backButtonHeight = 60;
    // Assign the rectangle to the 'Back' button object in optionsButtons for click detection
    optionsButtons[2].rect = { panelRect.x + (panelWidth - backButtonWidth) / 2, panelRect.y + panelHeight - 100, (float)backButtonWidth, (float)backButtonHeight };

    // Draw the visual elements of the "Back" button
    DrawRectangleRounded(optionsButtons[2].rect, 0.5f, 8, {60, 80, 100, 255});
    Vector2 backTextSize = MeasureTextEx(customFont, backText, 36, 1);
    DrawTextEx(customFont, backText, {optionsButtons[2].rect.x + (backButtonWidth - backTextSize.x) / 2, optionsButtons[2].rect.y + (backButtonHeight - backTextSize.y) / 2}, 36, 1, RAYWHITE);
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

void UIManager::DrawLevelSelection() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    ClearBackground({22, 28, 40, 255}); // Dark blue-gray background

    // Main UI panel box
    int panelWidth = 600;
    int panelHeight = 600;
    Rectangle panelRect = {(float)(screenWidth - panelWidth) / 2, (float)(screenHeight - panelHeight) / 2, (float)panelWidth, (float)panelHeight};
    Color panelColor = {35, 45, 60, 230};
    Color panelBorderColor = {60, 80, 100, 255};

    DrawRectangleRounded(panelRect, 0.5f, 10, panelColor);
    DrawRectangleRoundedLines(panelRect, 0.5f, 2, panelBorderColor);

    // Glowing title
    const char *titleText = "SELECT DIFFICULTY";
    int fontSize = 56;
    Vector2 textSize = MeasureTextEx(customFont, titleText, fontSize, 2);
    Vector2 titlePos = {panelRect.x + (panelWidth - textSize.x) / 2, panelRect.y + 50};
    for (int i = 4; i >= 1; i--) {
        float alpha = 0.1f * i;
        DrawTextEx(customFont, titleText, {titlePos.x, titlePos.y}, fontSize, 2, Fade(Color{100, 150, 255, 255}, alpha));
    }
    DrawTextEx(customFont, titleText, titlePos, fontSize, 2, RAYWHITE);

    // Modern UI layout variables for level buttons
    int buttonsYStart = panelRect.y + 150;
    int buttonWidth = 350;
    int buttonHeight = 70;
    int buttonSpacing = 25;
    int buttonX = panelRect.x + (panelWidth - buttonWidth) / 2;
    Vector2 mousePoint = GetMousePosition();

    // Draw Easy, Medium, and Hard buttons (first 3 elements)
    for (int i = 0; i < 3; i++) {
        // Ensure the vector has at least this many elements before trying to access them.
        if (i >= levelButtons.size()) {
            break; // Stop if we run out of buttons.
        }
        Button &btn = levelButtons[i];
        btn.rect = {(float)buttonX, (float)(buttonsYStart + i * (buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight};

        Color buttonColor = {60, 80, 100, 255};
        if (CheckCollisionPointRec(mousePoint, btn.rect)) {
            buttonColor = {80, 100, 120, 255};
        }
        DrawRectangleRounded(btn.rect, 0.5f, 8, buttonColor);

        Vector2 textPos = {btn.rect.x + (btn.rect.width - MeasureTextEx(customFont, btn.text.c_str(), 36, 1).x) / 2, btn.rect.y + (btn.rect.height - MeasureTextEx(customFont, btn.text.c_str(), 36, 1).y) / 2};
        DrawTextEx(customFont, btn.text.c_str(), textPos, 36, 1, RAYWHITE);
    }

    // Back button (the last element)
    if (levelButtons.size() >= 4) {
        Button &backBtn = levelButtons[3];
        backBtn.rect = {panelRect.x + (panelWidth - 200) / 2, panelRect.y + panelHeight - 80, 200.0f, 60.0f};

        Color backButtonColor = {60, 80, 100, 255};
        if (CheckCollisionPointRec(mousePoint, backBtn.rect)) {
            backButtonColor = {80, 100, 120, 255};
        }
        DrawRectangleRounded(backBtn.rect, 0.5f, 8, backButtonColor);

        Vector2 backTextSize = MeasureTextEx(customFont, backBtn.text.c_str(), 36, 1);
        DrawTextEx(customFont, backBtn.text.c_str(), {backBtn.rect.x + (200 - backTextSize.x) / 2, backBtn.rect.y + (60 - backTextSize.y) / 2}, 36, 1, RAYWHITE);
    }
}void UIManager::DrawHowToPlay() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    ClearBackground({10, 12, 20, 255});  // Deep dark background

    // --- Main Frosted Panel (Bigger & More Prominent) ---
    int panelWidth = 880;
    int panelHeight = 720;
    Rectangle panel = {
        (float)(screenWidth - panelWidth) / 2,
        (float)(screenHeight - panelHeight) / 2,
        (float)panelWidth,
        (float)panelHeight
    };

    Color glassBg = {25, 30, 45, 235};      // Soft dark glass tone
    Color glowEdge = {100, 200, 255, 40};   // Glow border

    // Multi-layered soft glow effect for panel
    for (int i = 7; i > 0; i--) {
        DrawRectangleRounded(panel, 0.10f, 18, Fade(glowEdge, 0.03f * i));
    }
    DrawRectangleRounded(panel, 0.10f, 18, glassBg);
    DrawRectangleRoundedLines(panel, 0.10f, 2, glowEdge);

    // --- Header Title ---
    const char* title = "HOW TO PLAY";
    int titleFontSize = 66;
    Vector2 titleSize = MeasureTextEx(customFont, title, titleFontSize, 2);
    Vector2 titlePos = {
        panel.x + (panel.width - titleSize.x) / 2,
        panel.y + 40
    };

    for (int i = 4; i > 0; i--) {
        DrawTextEx(customFont, title, titlePos, titleFontSize, 2, Fade({120, 210, 255, 255}, 0.08f * i));
    }
    DrawTextEx(customFont, title, titlePos, titleFontSize, 2, RAYWHITE);

    // --- Instruction Cards (Modern Style, Bigger & Colorful) ---
    const char* rules[] = {
        "Left Click  -  Reveal a tile",
        "Right Click -  Flag a tile",
        "Uncover all safe tiles to win",
        "Numbers indicate nearby mines",
        "Revealing a mine ends the game",
        "Press ENTER to return to menu"
    };

    Color cardColors[] = {
        {70, 130, 255, 255},
        {255, 99, 132, 255},
        {75, 192, 192, 255},
        {255, 206, 86, 255},
        {220, 53, 69, 255},
        {153, 102, 255, 255}
    };

    int cardX = panel.x + 60;
    int cardY = panel.y + 130;
    int cardW = panel.width - 120;
    int cardH = 90;           // Increased card height
    int gapY = 26;            // Increased vertical gap
    int fontSize = 32;        // Larger text

    for (int i = 0; i < 6; i++) {
        Rectangle card = {
            (float)cardX,
            (float)(cardY + i * (cardH + gapY)),
            (float)cardW,
            (float)cardH
        };

        // Soft glow effect behind card
        for (int j = 3; j > 0; j--) {
            DrawRectangleRounded(card, 0.25f, 16, Fade(cardColors[i], 0.05f * j));
        }

        // Card background & outline
        DrawRectangleRounded(card, 0.25f, 16, Fade(cardColors[i], 225));
        DrawRectangleRoundedLines(card, 0.25f, 2, Fade(WHITE, 60));

        // Instruction Text
        DrawTextEx(customFont, rules[i], {
            card.x + 28,
            card.y + (card.height - fontSize) / 2
        }, fontSize, 1, RAYWHITE);
    }
}
