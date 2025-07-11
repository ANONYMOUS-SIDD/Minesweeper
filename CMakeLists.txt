cmake_minimum_required(VERSION 3.21)
project(SignupApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Uncomment and set this if you want to hardcode vcpkg toolchain file here:
# set(CMAKE_TOOLCHAIN_FILE "C:/Users/Siddhant/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain")

# Find raylib and cpr packages installed via vcpkg
find_package(raylib CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

# Add executable target and include the signup class file
add_executable(signup main.cpp sweep.cpp SplashScreen.cpp Backend_Integration/signup_class.cpp Backend_Integration/login_class.cpp Backend_Integration/score_class.cpp SignUpScreen.cpp LoginScreen.cpp home_screen.cpp Backend_Integration/leader_board.cpp)


# Include the Backend_Integration directory so it can find signup_class.h
target_include_directories(signup PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Backend_Integration
)

# Link raylib and cpr libraries
target_link_libraries(signup PRIVATE raylib cpr::cpr)

# Link extra system libraries needed by raylib on Windows
if(WIN32)
    target_link_libraries(signup PRIVATE winmm gdi32 opengl32 ws2_32)
endif()
