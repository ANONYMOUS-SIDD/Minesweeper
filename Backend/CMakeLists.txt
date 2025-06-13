cmake_minimum_required(VERSION 3.15)
project(hello_crow)

set(CMAKE_CXX_STANDARD 20)

# Path to vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Siddhant/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Define Windows version macro
add_definitions(-D_WIN32_WINNT=0x0601)

# Find required packages
find_package(Crow CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)  # For password hashing
find_package(CURL CONFIG REQUIRED)  # Added for libcurl HTTP client

# Add executable with your source files here (change main.cpp if needed)
add_executable(main main.cpp)

# Link libraries to your executable
target_link_libraries(main PRIVATE
    Crow::Crow
    asio::asio
    libpqxx::pqxx
    unofficial-sodium::sodium
    CURL::libcurl    # Link curl library
)
