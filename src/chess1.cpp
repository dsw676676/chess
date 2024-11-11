#include <iostream>
#include <vector>
using namespace std;

class Board
{
private:
    vector<vector<char>> board;
    int size;

public:
    Board(int n) : size(n)
    {
        board.resize(size, vector<char>(size, '.'));
    }
    char getIn(int x, int y)
    {
        if (x >= 0 && x < size && y >= 0 && y < size)
        {
            return board[x][y];
        }
        return 'E';
    }
    void display()
    {
        cout << string(2, ' '); // 第一行
        for (int y = 0; y < size; y++)
        {
            cout << y % 10 << ' ';
        }
        cout << endl;
        for (int x = 0; x < size; x++)
        {
            cout << x % 10 << ' '; // 第一列
            for (int y = 0; y < size; y++)
            {
                cout << board[x][y] << ' '; // 棋盘点位
            }
            cout << endl;
        }
    }
    vector<pair<int, int>> getAvailableMoves()
    {
        vector<pair<int, int>> availableMoves;
        for (int x = 0; x < size; x++)
        {
            for (int y = 0; y < size; y++)
            {
                if (board[x][y] == '.')
                {
                    availableMoves.emplace_back(x, y);
                }
            }
        }
        return availableMoves;
    }
    bool setIn(int x, int y, char piece)
    {
        if (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == '.')
        {
            board[x][y] = piece;
            return true;
        }
        return false;
    }
};

class Game
{
private:
    Board board;
    int x, y;
    char humanPlayer;
    char currentPlayer = 'X';
    void chooseHumanPlayer()
    {
        cout << "Choose your turn (X or O): " << endl;
        while (true)
        {
            cin >> humanPlayer;
            if (humanPlayer == 'X' || humanPlayer == 'O')
            {
                break;
            }
            cout << "Invalid turn. Try again." << endl;
        }
    }
    int countConsecutive(int x, int y, int dx, int dy, char piece) // 检查连子数量
    {
        int count = 0;
        for (int step = 1; step < 5; step++)
        {
            int newX = x + step * dx;
            int newY = y + step * dy;
            if (board.getIn(newX, newY) == piece)
            {
                count++;
            }
            else
            {
                break;
            }
        }
        return count;
    };
    bool checkWin(int x, int y) // 胜负判断
    {
        if (countConsecutive(x, y, 0, 1, currentPlayer) + countConsecutive(x, y, 0, -1, currentPlayer) >= 4 || countConsecutive(x, y, 1, 0, currentPlayer) + countConsecutive(x, y, -1, 0, currentPlayer) >= 4 || countConsecutive(x, y, 1, 1, currentPlayer) + countConsecutive(x, y, -1, -1, currentPlayer) >= 4 || countConsecutive(x, y, 1, -1, currentPlayer) + countConsecutive(x, y, -1, 1, currentPlayer) >= 4)
        {
            return true;
        }
        return false;
    };
    pair<int, int> humanMove()
    {
        cout << "Player " << currentPlayer << ", enter your move (row and column): " << endl;
        while (true)
        {
            // 输入
            cin >> x >> y;
            // 判别
            if (board.setIn(x, y, currentPlayer))
            {
                break;
            }
            cout << "Invalid move. Try again." << endl;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore('\n');
            }
        }
        return pair(x, y);
    }
    int evaluatePosition(int x, int y, char piece)
    {
        int score = 0;
        // 检查所有方向
        score += evaluateLine(x, y, 0, 1, piece);  // 横向
        score += evaluateLine(x, y, 1, 0, piece);  // 纵向
        score += evaluateLine(x, y, 1, 1, piece);  // 主对角线
        score += evaluateLine(x, y, 1, -1, piece); // 副对角线
        return score;
    }
    int evaluateLine(int x, int y, int dx, int dy, char piece)
    {
        int count = 1; // 中间棋子
        int block = 0; // 封闭端点数
        int empty = 0; // 空位数量

        // 向一个方向计数
        int tx = x + dx;
        int ty = y + dy;
        while (board.getIn(tx, ty) == piece)
        {
            count++;
            tx += dx;
            ty += dy;
        }
        if (board.getIn(tx, ty) == '.')
        {
            empty++;
        }
        else if (board.getIn(tx, ty) != 'E')
        {
            block++;
        }

        // 向另一个方向计数
        tx = x - dx;
        ty = y - dy;
        while (board.getIn(tx, ty) == piece)
        {
            count++;
            tx -= dx;
            ty -= dy;
        }
        if (board.getIn(tx, ty) == '.')
        {
            empty++;
        }
        else if (board.getIn(tx, ty) != 'E')
        {
            block++;
        }

        // 根据连子数和封闭端点数评分
        if (count >= 5)
        {
            return 100000; // 直接获胜
        }
        else if (block == 0)
        {
            if (count == 4)
                return 10000; // 活四
            if (count == 3 && empty == 2)
                return 1000; // 活三
            if (count == 2 && empty == 2)
                return 100; // 活二
        }
        else if (block == 1)
        {
            if (count == 4)
                return 1000; // 半活四
            if (count == 3 && empty == 1)
                return 100; // 半活三
            if (count == 2 && empty == 2)
                return 10; // 半活二
        }
        return 0;
    }
    pair<int, int> robotMove()
    {
        auto availableMoves = board.getAvailableMoves();
        int bestScore = -1;
        pair<int, int> bestMove;

        for (const auto &move : availableMoves)
        {
            int score = 0;
            int x = move.first;
            int y = move.second;

            // 计算在(x, y)位置下棋的分数，而不真正下棋
            score += evaluatePosition(x, y, currentPlayer) * 10;                     // 当前玩家的得分
            score += evaluatePosition(x, y, (currentPlayer == 'X') ? 'O' : 'X') * 5; // 对手的得分（防守）

            // 更新最佳移动
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }

        // 执行最佳移动
        if (board.setIn(bestMove.first, bestMove.second, currentPlayer))
        {
            cout << "Robot plays at: (" << bestMove.first << ", " << bestMove.second << ")" << endl;
        }
        return bestMove;
    }

public:
    Game(int size) : board(size)
    {
    } // 游戏构建-构造函数

    void play() // 棋局运行
    {
        pair<int, int> move;
        // 选择玩家
        chooseHumanPlayer();
        while (true)
        {
            // 显示与着棋
            board.display();
            if (currentPlayer == humanPlayer)
            {
                move = humanMove();
            }
            else
            {
                move = robotMove();
            }
            // 胜败判别
            if (checkWin(move.first, move.second))
            {
                board.display();
                cout << "Player " << currentPlayer << " wins!" << endl;
                break;
            }
            // 棋手切换
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
};

int main()
{
    Game game(15);
    game.play();
    return 0;
}