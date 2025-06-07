#!/bin/bash
# Build the project
gcc main.c -o hello_raylib -lraylib -lopengl32 -lgdi32 -lwinmm

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Build successful! Running the program..."
    ./hello_raylib
else
    echo "Build failed. Please check for errors."

