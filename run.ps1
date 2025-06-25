#!/bin/bash

# Build and run a C++ raylib app
g++ main.cpp -o hello_raylib -lraylib -lopengl32 -lgdi32 -lwinmm

if [ $? -eq 0 ]; then
    echo "Build successful! Running the program..."
    ./hello_raylib
else
    echo "Build failed. Please check for errors."
fi
