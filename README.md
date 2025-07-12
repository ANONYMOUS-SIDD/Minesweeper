# Raylib Project Setup & Run Guide on Windows (Using MSYS2 UCRT Terminal)

Welcome to the **Raylib setup and run guide**! This document will walk you through installing MSYS2, setting up Raylib, and creating your first Raylib project on **Windows** using the **MSYS2 UCRT terminal**.

---

## 📌 What is Raylib?

Raylib is a simple and easy-to-use C library designed to help you learn game programming and graphics development. It's lightweight, beginner-friendly, and ideal for small game projects and graphical demos.

---

## ✅ Step 1: Install MSYS2

MSYS2 is a minimalist Unix-like environment for Windows, providing essential development tools via the Pacman package manager. We will use the **UCRT64 environment** of MSYS2, which supports modern Windows development.

### Installation Instructions:

1. Visit [https://www.msys2.org/](https://www.msys2.org/)
2. Download the installer: `msys2-x86_64-<version>.exe`
3. Run the installer and follow on-screen instructions.
4. After installation, launch the **MSYS2 MSYS Terminal** from the Start menu.

### Update MSYS2:

```bash
pacman -Syu
```

> ⚠️ After updating, **close** and **reopen** the terminal.

### Install UCRT64 environment and development tools:

Open **MSYS2 MSYS Terminal** and run:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-raylib
```

### Use the UCRT64 Environment:

From the Start menu, open **MSYS2 UCRT64 Terminal**. All further steps will be done from this terminal.

---

## ✅ Step 2: Create Your Raylib Project Directory

Open the **MSYS2 UCRT64 Terminal** and run:

```bash
mkdir -p hello_raylib
cd hello_raylib
```

- `mkdir -p` creates the folder (and any missing parent folders).
- `cd` navigates into the project directory.

---

## ✅ Step 3: Create Your First Raylib Program

Create a new file called `main.c` with the following code:
You can use any text editor, or simply run:

```bash
nano main.c
```

```c
#include "raylib.h"

int main(void)
{
    // Initialize the window
    InitWindow(800, 600, "Hello Raylib");

    // Set the target frames per second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello Raylib!", 190, 200, 40, LIGHTGRAY);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}
```

---

## ✅ Step 4: Create a Build and Run Script

For convenience, create a shell script called `run.sh`:

```bash
nano run.sh
```

Paste the following content:

```bash
#!/bin/bash
# Build the project
gcc main.c -o hello_raylib -lraylib -lopengl32 -lgdi32 -lwinmm

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Build successful! Running the program..."
    ./hello_raylib
else
    echo "Build failed. Please check for errors."
fi
```

Make the script executable:

```bash
chmod +x run.sh
```

Run the script:

```bash
./run.sh
```

---

## ✅ Step 5: Compile and Run Manually (Optional)

If you prefer to compile without the script:

```bash
gcc main.c -o hello_raylib -lraylib -lopengl32 -lgdi32 -lwinmm && ./hello_raylib
```

**Explanation:**

- `gcc`: GNU C Compiler
- `-o hello_raylib`: Output file name
- `-lraylib`: Link Raylib
- `-lopengl32 -lgdi32 -lwinmm`: Link necessary Windows libraries
- `&& ./hello_raylib`: Run the program if compilation succeeds

---

## 🛠️ Troubleshooting

**Problem:** Raylib not found during compilation?

- Make sure Raylib is installed:

```bash
pacman -S mingw-w64-ucrt-x86_64-raylib
```

- If errors persist, update MSYS2:

```bash
pacman -Syu
```

> Restart the UCRT64 terminal after updating to refresh environment settings.

---

## 📋 Summary of Commands

```bash
# Create and enter project directory
mkdir -p hello_raylib
cd hello_raylib

# Create source file
nano main.c

# Create build/run script
nano run.sh
chmod +x run.sh

# Run the script
./run.sh
```

---

## 🔗 Additional Resources

- Raylib Website: [https://www.raylib.com](https://www.raylib.com)
- Raylib GitHub: [https://github.com/raysan5/raylib](https://github.com/raysan5/raylib)
- MSYS2: [https://www.msys2.org](https://www.msys2.org)

---

## 💡 Tips

- Modify `main.c` freely to learn and experiment with Raylib features.
- Always use the **MSYS2 UCRT64 Terminal** to build and run Raylib projects.
- To exit a Raylib window, simply click the close button or press `ESC`.
- The `run.sh` script simplifies your workflow by combining build and execution steps.

---

Happy coding and good luck with your Raylib projects! 🎮
