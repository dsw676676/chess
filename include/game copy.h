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
    char humanPiece;
    int difficulty;
    char currentPiece = 'X';

    HumanPlayer humanPlayer;
    RobotPlayer robotPlayer;
    Referee referee;
    Board board;

    void initializeGame()
    {
        // 选择玩家
        int humanPieceType;
        cout << "\033[2J\033[H";
        cout << "Choose your piece (1 or 2): " << endl;
        cout << "  1. Black piece: 'X'." << endl;
        cout << "  2. White piece: 'O'." << endl;
        while (true)
        {
            cin >> humanPieceType;
            if (humanPieceType == 1 || humanPieceType == 2)
            {
                humanPiece = (humanPieceType == 1) ? 'X' : 'O';
                humanPlayer.setPiece(humanPiece);
                break;
            }
            cout << "Invalid piece. Try again." << endl;
        }
        // 选择难度
        cout << "\033[2J\033[H";
        cout << "Choose the robot difficulty (1 or 2): " << endl;
        cout << "  1. Easy." << endl;
        cout << "  2. Hard." << endl;
        while (true)
        {
            cin >> difficulty;
            if (difficulty == 1 || difficulty == 2)
            {
                robotPlayer.setPiece((humanPieceType == 1) ? 'O' : 'X', difficulty - 1);
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
                board.display();
                cout << "Player " << currentPiece << " wins!" << endl;
                break;
            }
            // 棋手切换
            currentPiece = (currentPiece == 'X') ? 'O' : 'X';
            turn ++;
        }
    }
};

class RobotRobotGame
{
private:
    int difficulty1;
    int difficulty2;
    char currentPiece = 'X';

    RobotPlayer robotPlayer1;
    RobotPlayer robotPlayer2;
    Referee referee;
    Board board;

    void initializeGame()
    {
        // 选择难度1
        cout << "Choose the robot1 difficulty (0 or 1): " << endl;
        while (true)
        {
            cin >> difficulty1;
            if (difficulty1 == 0 || difficulty1 == 1)
            {
                robotPlayer1.setPiece('X', difficulty1);
                break;
            }
            cout << "Invalid difficulty. Try again." << endl;
        }
        // 选择难度2
        cout << "Choose the robot2 difficulty (0 or 1): " << endl;
        while (true)
        {
            cin >> difficulty2;
            if (difficulty2 == 0 || difficulty2 == 1)
            {
                robotPlayer2.setPiece('O', difficulty2);
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
            cout << endl;
            cout << string(40, '-') << endl;
            cout << "Turn " << turn << " - Player " << currentPiece << ":" << endl;
            board.display();
            if (currentPiece == 'X')
            {
                board = robotPlayer1.move(board);
            }
            else
            {
                board = robotPlayer2.move(board);
            }
            // 胜败判别
            if (referee.checkWin(board, currentPiece))
            {
                board.display();
                cout << "Player " << currentPiece << " wins!" << endl;
                break;
            }
            // 棋手切换
            currentPiece = (currentPiece == 'X') ? 'O' : 'X';
            turn ++;
        }
    }
};

#endif