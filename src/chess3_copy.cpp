#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include "../include/player.h"
#include "../include/game.h"

class GomokuWidget : public QWidget
{
private:
    int size = 15;
    char humanPiece = 'X';
    char robotPiece = 'O';
    Board board = Board(size);
    HumanPlayer humanPlayer;
    RobotPlayer robotPlayer;
    Referee referee;
    QPushButton *buttons[15][15]; // 假设棋盘大小为15x15

    void updateBoardDisplay()
    {
        for (int row = 0; row < board.size; ++row)
        {
            for (int col = 0; col < board.size; ++col)
            {
                char piece = board.getIn(row, col);
                buttons[row][col]->setText(piece == '.' ? "" : QString(piece));
            }
        }
    }

public slots:
    void onCellClicked(int row, int col)
    {
        if (board.isInboundsAndEmpty(row, col))
        {
            board.setIn(row, col, humanPiece);
            updateBoardDisplay();
            if (referee.checkWin(board, humanPiece))
            {
                // 游戏结束
                cout << "Player" << humanPiece << "wins!";
                return;
            }
            // 机器人移动
            board = robotPlayer.move(board);
            updateBoardDisplay();
            if (referee.checkWin(board, robotPiece)) {
                // 游戏结束
                cout << "Player" << robotPiece << "wins!";
            }
        }
    }

public:
    GomokuWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        humanPlayer.setPiece('X');
        robotPlayer.setPiece('O', 1);
        QGridLayout *layout = new QGridLayout(this);
        for (int row = 0; row < board.size; ++row)
        {
            for (int col = 0; col < board.size; ++col)
            {
                QPushButton *button = new QPushButton(this);
                button->setFixedSize(40, 40); // 设置按钮大小
                connect(button, &QPushButton::clicked, this, [=]()
                        { onCellClicked(row, col); });
                layout->addWidget(button, row, col);
                buttons[row][col] = button;
            }
        }
        updateBoardDisplay();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GomokuWidget widget;
    widget.show();
    return app.exec();
}

// class Game
// {
// private:
//     int size;
//     int gameType;
//     void chooseGameType()
//     {
//         // 选择游戏类型
//         cout << "Choose the game type (1 or 2): " << endl;
//         cout << "  1. Human-Machine competition" << endl;
//         cout << "  2. Machine-Machine competition" << endl;
//         while (true)
//         {
//             cin >> gameType;
//             if (gameType == 1 || gameType == 2)
//             {
//                 break;
//             }
//             cout << "Invalid piece. Try again." << endl;
//         }
//     }
//     bool chooseRestart()
//     {
//         // 选择是否重新开始
//         char isRestart;
//         cout << "Do you want to restart? (y or n): " << endl;
//         while (true)
//         {
//             cin >> isRestart;
//             if (isRestart == 'y' || isRestart == 'n')
//             {
//                 break;
//             }
//             cout << "Invalid input. Try again." << endl;
//         }
//         return isRestart == 'y';
//     }

// public:
//     Game(int n) : size(n) {}
//     void start()
//     {
//         while (true)
//         {
//             chooseGameType();
//             if (gameType == 1)
//             {
//                 HumanRobotGame humanRobotGame(size);
//                 humanRobotGame.play();
//             }
//             else if (gameType == 2)
//             {
//                 RobotRobotGame robotRobotGame(size);
//                 robotRobotGame.play();
//             }
//             if (! chooseRestart())
//             {
//                 break;
//             }
//         }
//     }
// };

// int main(int argc, char const *argv[])
// {
//     Game game(15);
//     game.start();
//     return 0;
// }
