cmake_minimum_required(VERSION 3.10)
project(chess_game)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)

# 添加可执行文件
include_directories(include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(chess_game_linux src/chess3.cpp)
# add_executable(chess_game_windows.exe src/chess3.cpp)

# 指定交叉编译工具链文件
set(CMAKE_TOOLCHAIN_FILE cmake/LinuxToolchain.cmake)
# set(CMAKE_TOOLCHAIN_FILE cmake/WindowsToolchain.cmake)