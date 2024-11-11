#ifndef GAME
#define GAME

class HumanRobotGame;
class RobotRobotGame;

#include <iostream>
#include "player.h"
using namespace std;

class HumanRobotGame
{
private:
    int humanPiece;
    int difficulty;
    int currentPiece = 1;

    HumanPlayer humanPlayer;
    RobotPlayer robotPlayer;
    Referee referee;
    Board board;

    void initializeGame()
    {
        // 选择玩家
        cout << "\033[2J\033[H";
        cout << "Choose your piece (1 or 2): " << endl;
        cout << "  1. Black piece: 'X'." << endl;
        cout << "  2. White piece: 'O'." << endl;
        while (true)
        {
            cin >> humanPiece;
            if (humanPiece == 1 || humanPiece == 2)
            {
                humanPlayer.setPiece(humanPiece);
                break;
            }
            cout << "Invalid piece. Try again." << endl;
        }
        // 选择难度
        cout << "\033[2J\033[H";
        cout << "Choose the robot1 difficulty (1 or 2 or 3): " << endl;
        cout << "  1. Easy." << endl;
        cout << "  2. Medium." << endl;
        cout << "  3. Hard." << endl;
        while (true)
        {
            cin >> difficulty;
            if (difficulty == 1 || difficulty == 2 || difficulty == 3)
            {
                robotPlayer.setPiece((humanPiece == 1) ? 2 : 1, difficulty);
                break;
            }
            cout << "Invalid difficulty. Try again." << endl;
        }
    }

public:
    HumanRobotGame(int n) : board(n) {}
    void play()
    {
        // 初始化设置
        initializeGame();
        int turn = 1;
        while (true)
        {
            // 显示与着棋
            cout << "\033[2J\033[H";
            cout << "Turn " << turn << " - Player " << currentPiece << ":" << endl;
            board.display();
            if (currentPiece == humanPiece)
            {
                board = humanPlayer.move(board);
            }
            else
            {
                board = robotPlayer.move(board);
            }
            // 胜败判别
            if (referee.checkWin(board, currentPiece))
            {
                cout << "\033[2J\033[H";
                cout << "Player " << currentPiece << " wins!" << endl;
                board.display();
                break;
            }
            // 棋手切换
            currentPiece = (currentPiece == 1) ? 2 : 1;
            turn ++;
        }
    }
};

class RobotRobotGame
{
private:
    int difficulty1;
    int difficulty2;
    int currentPiece = 1;

    RobotPlayer robotPlayer1;
    RobotPlayer robotPlayer2;
    Referee referee;
    Board board;

    void initializeGame()
    {
        // 选择难度1
        cout << "\033[2J\033[H";
        cout << "Choose the robot1 difficulty (1 or 2 or 3): " << endl;
        cout << "  1. Easy." << endl;
        cout << "  2. Medium." << endl;
        cout << "  3. Hard." << endl;
        while (true)
        {
            cin >> difficulty1;
            if (difficulty1 == 1 || difficulty1 == 2 || difficulty1 == 3)
            {
                robotPlayer1.setPiece(1, difficulty1);
                break;
            }
            cout << "Invalid difficulty. Try again." << endl;
        }
        // 选择难度2
        cout << "\033[2J\033[H";
        cout << "Choose the robot2 difficulty (1 or 2 or 3): " << endl;
        cout << "  1. Easy." << endl;
        cout << "  2. Medium." << endl;
        cout << "  3. Hard." << endl;
        while (true)
        {
            cin >> difficulty2;
            if (difficulty2 == 1 || difficulty2 == 2 || difficulty2 == 3)
            {
                robotPlayer2.setPiece(2, difficulty2);
                break;
            }
            cout << "Invalid difficulty. Try again." << endl;
        }
    }

public:
    RobotRobotGame(int n) : board(n) {}
    void play()
    {
        // 初始化设置
        initializeGame();
        int turn = 1;
        while (true)
        {
            // 显示与着棋
            cout << "\033[2J\033[H";
            cout << "Turn " << turn << " - Player " << currentPiece << ":" << endl;
            board.display();
            if (currentPiece == 1)
            {
                board = robotPlayer1.move(board);
            }
            else if (currentPiece == 2)
            {
                board = robotPlayer2.move(board);
            }
            // 胜败判别
            if (referee.checkWin(board, currentPiece))
            {
                cout << "\033[2J\033[H";
                cout << "Player " << currentPiece << " wins!" << endl;
                board.display();
                break;
            }
            // 棋手切换
            currentPiece = (currentPiece == 1) ? 2 : 1;
            turn ++;
        }
    }
};

#endif