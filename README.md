# 五子棋项目

## 项目文件

```
.
├── bin
│   ├── chess_game_linux
│   └── chess_game_windows.exe
├── cmake
│   ├── LinuxToolchain.cmake
│   └── WindowsToolchain.cmake
├── CMakeLists.txt
├── include
│   ├── game.h
│   └── player.h
├── README.md
└── src
    ├── chess1.cpp
    ├── chess2.cpp
    └── chess3.cpp
```

## 运行方式

- 构建：项目基于`cmake`构建，基于`./CMakeLists.txt`文件。原编译平台为linux x86-64，但在windows x86-64上也能够运行。
- 运行：二进制文件为`./bin/chess_game`，可直接运行。
