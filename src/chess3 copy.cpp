#include <iostream>
#include "../include/player.h"
#include "../include/game.h"

class Game
{
private:
    int size;
    int gameType;
    void chooseGameType()
    {
        // 选择游戏类型
        cout << "\033[2J\033[H";
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
            cout << "Invalid piece. Try again." << endl;
        }
    }
    bool chooseRestart()
    {
        // 选择是否重新开始
        char isRestart;
        cout << "\033[2J\033[H";
        cout << "Do you want to restart? (y or n): " << endl;
        while (true)
        {
            cin >> isRestart;
            if (isRestart == 'y' || isRestart == 'n')
            {
                break;
            }
            cout << "Invalid input. Try again." << endl;
        }
        return isRestart == 'y';
    }

public:
    Game(int n) : size(n) {}
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
    Game game(15);
    game.start();
    return 0;
}
