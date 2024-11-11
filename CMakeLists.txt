cmake_minimum_required(VERSION 3.10)
project(chess_game)

# 设置标准
set(CMAKE_CXX_STANDARD 11)

# 输入设置
# find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
include_directories("include")

# 输出设置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(chess_game src/chess3.cpp)

# 链接静态库
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)
# target_link_libraries(chess_game
#     PRIVATE Qt6::Core
#     PRIVATE Qt6::Gui
#     PRIVATE Qt6::Widgets
# )