#include <iostream>
#include <cstdlib>
#include "../include/player.h"
#include "../include/game.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

class Game
{
private:
    int size;
    int gameType;
    void chooseGameType()
    {
        // 选择游戏类型
        clearScreen();
        cout << "Choose the game type (1 or 2): " << endl;
        cout << "  1. Human-Machine competition" << endl;
        cout << "  2. Machine-Machine competition" << endl;
        while (true)
        {
            cin >> gameType;
            if (gameType == 1 || gameType == 2)
            {
                break;
            }
            cout << "Invalid size. Try again." << endl;
        }
        // 选择游戏大小
        clearScreen();
        cout << "Choose the game size (8 - 20): " << endl;
        while (true)
        {
            cin >> size;
            if (size >= 8 && size <= 20)
            {
                break;
            }
            cout << "Invalid piece. Try again." << endl;
        }
    }
    bool chooseRestart()
    {
        // 选择是否重新开始
        int isRestart;
        clearScreen();
        cout << "Do you want to restart? (1 or 2): " << endl;
        cout << "  1. Yes." << endl;
        cout << "  2. No." << endl;
        while (true)
        {
            cin >> isRestart;
            if (isRestart == 1 || isRestart == 2)
            {
                break;
            }
            cout << "Invalid input. Try again." << endl;
        }
        return isRestart == 1;
    }

public:
    void start()
    {
        while (true)
        {
            chooseGameType();
            if (gameType == 1)
            {
                HumanRobotGame humanRobotGame(size);
                humanRobotGame.play();
            }
            else if (gameType == 2)
            {
                RobotRobotGame robotRobotGame(size);
                robotRobotGame.play();
            }
            if (! chooseRestart())
            {
                break;
            }
        }
    }
};

int main(int argc, char const *argv[])
{
    Game game;
    game.start();
    return 0;
}
