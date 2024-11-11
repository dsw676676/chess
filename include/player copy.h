#ifndef PLAYER
#define PLAYER

class Board;
class HumanPlayer;
class RobotPlayer;
class Referee;

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Board
{
private:
    vector<vector<char>> board;

public:
    int size;
    Board(int n) : size(n)
    {
        board.resize(size, vector<char>(size, '.'));
    }
    bool isInbounds(int x, int y)
    {
        if (x >= 0 && x < size && y >= 0 && y < size)
        {
            return true;
        }
        return false;
    }
    bool isInboundsAndEmpty(int x, int y)
    {
        if (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == '.')
        {
            return true;
        }
        return false;
    }
    char getIn(int x, int y)
    {
        if (x >= 0 && x < size && y >= 0 && y < size)
        {
            return board[x][y];
        }
        return 'E';
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
    void setIn(int x, int y, char piece)
    {
        board[x][y] = piece;
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
};

class HumanPlayer
{
private:
    char piece;

public:
    HumanPlayer() {}
    void setPiece(char choosePiece)
    {
        piece = choosePiece;
    }
    Board move(Board board)
    {
        int x, y;
        cout << "Enter your move (row and column): " << endl;
        while (true)
        {
            // 输入
            cin >> x >> y;
            // 判别
            if (board.isInboundsAndEmpty(x, y))
            {
                board.setIn(x, y, piece);
                break;
            }
            cout << "Invalid move. Try again." << endl;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore('\n');
            }
        }
        return board;
    }
};

class RobotPlayer
{
private:
    char piece;
    char opponentPiece;
    int depth;

    int evaluateLine(Board board, int x, int y, int dx, int dy, char piece)
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
        if (board.isInboundsAndEmpty(tx, ty))
        {
            empty++;
        }
        else
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
        if (board.isInboundsAndEmpty(tx, ty))
        {
            empty++;
        }
        else
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
    int evaluatePosition(Board board, int x, int y, char piece)
    {
        int score = 0;
        // 检查所有方向
        score += board.size - max(abs(x - board.size / 2), abs(y - board.size / 2)) * 2;
        score += evaluateLine(board, x, y, 0, 1, piece);  // 横向
        score += evaluateLine(board, x, y, 1, 0, piece);  // 纵向
        score += evaluateLine(board, x, y, 1, 1, piece);  // 主对角线
        score += evaluateLine(board, x, y, 1, -1, piece); // 副对角线
        return score;
    }
    int evaluateBoard(Board board, char piece)
    {
        int score = 0;
        int opponentScore = 0;
        // 遍历棋盘，评估每个位置
        for (int i = 0; i < board.size; ++i)
        {
            for (int j = 0; j < board.size; ++j)
            {
                if (board.getIn(i, j) == piece)
                {
                    score += evaluatePosition(board, i, j, piece);
                }
                else if (board.getIn(i, j) == opponentPiece)
                {
                    opponentScore += evaluatePosition(board, i, j, opponentPiece);
                }
            }
        }
        // 返回当前玩家的得分减去对手的得分
        return score - opponentScore;
    }
    int minimax(Board board, int depth, bool isMaximizingPlayer, int alpha, int beta)
    {
        if (depth == 0)
        {
            return evaluateBoard(board, piece);
        }

        if (isMaximizingPlayer)
        {
            int maxEval = INT_MIN;
            for (const auto &move : board.getAvailableMoves())
            {
                board.setIn(move.first, move.second, piece);
                int eval = minimax(board, depth - 1, false, alpha, beta);
                board.setIn(move.first, move.second, '.');
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha)
                {
                    break; // β剪枝
                }
            }
            return maxEval;
        }
        else
        {
            int minEval = INT_MAX;
            for (const auto &move : board.getAvailableMoves())
            {
                board.setIn(move.first, move.second, opponentPiece);
                int eval = minimax(board, depth - 1, true, alpha, beta);
                board.setIn(move.first, move.second, '.');
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha)
                {
                    break; // α剪枝
                }
            }
            return minEval;
        }
    }

public:
    RobotPlayer() {}
    void setPiece(char choosePiece, int difficulty)
    {
        piece = choosePiece;
        opponentPiece = (choosePiece == 'X') ? 'O' : 'X';
        depth = difficulty;
    }
    Board move(Board board)
    {
        int bestScore = INT_MIN;
        pair<int, int> bestMove;

        for (const auto &move : board.getAvailableMoves())
        {
            board.setIn(move.first, move.second, piece);
            int score = minimax(board, depth, false, INT_MIN, INT_MAX);
            board.setIn(move.first, move.second, '.');

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }

        board.setIn(bestMove.first, bestMove.second, piece);
        cout << "Robot plays at: (" << bestMove.first << ", " << bestMove.second << ")" << endl;

        return board;
    }
};

class Referee
{
private:
    int countConsecutive(Board board, int x, int y, int dx, int dy, char piece) // 检查连子数量
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
    }

public:
    bool checkWin(Board board, char currentPiece) // 胜负判断
    {
        for (int i = 0; i < board.size; i++)
        {
            for (int j = 0; j < board.size; j++)
            {
                if (board.getIn(i, j) == currentPiece && (countConsecutive(board, i, j, 0, 1, currentPiece) + countConsecutive(board, i, j, 0, -1, currentPiece) >= 4 || countConsecutive(board, i, j, 1, 0, currentPiece) + countConsecutive(board, i, j, -1, 0, currentPiece) >= 4 || countConsecutive(board, i, j, 1, 1, currentPiece) + countConsecutive(board, i, j, -1, -1, currentPiece) >= 4 || countConsecutive(board, i, j, 1, -1, currentPiece) + countConsecutive(board, i, j, -1, 1, currentPiece) >= 4))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif