cmake_minimum_required(VERSION 3.15)
project(hello_crow)

set(CMAKE_CXX_STANDARD 20)

# Path to vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Siddhant/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Add the main source file
add_executable(main main.cpp)

# Find required packages via vcpkg
find_package(Crow CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)  # ✅ PostgreSQL C++ client

# Link libraries
target_link_libraries(main PRIVATE Crow::Crow asio::asio libpqxx::pqxx)
