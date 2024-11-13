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
    └── chess.cpp
```

## 运行方式

- 构建：项目基于`cmake`构建，基于`./CMakeLists.txt`文件。原编译平台为linux x86-64，但修改`./CMakeLists.txt`中toolchain也可以在windows x86-64上也能够运行。
- 运行：二进制文件为`./bin/chess_game_*platform*`，可直接运行。
