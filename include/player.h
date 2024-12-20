#ifndef PLAYER
#define PLAYER

class Board;
class HumanPlayer;
class RobotPlayer;
class Referee;

#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
using namespace std;

void clearScreen()
{
#if defined(_WIN32)
    system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    system("clear");
#endif
}

class Board
{
private:
    vector<vector<int>> board;

public:
    int size;
    Board(int n) : size(n)
    {
        board.resize(size, vector<int>(size, 0));
    }
    int getIn(int x, int y)
    {
        if (x >= 0 && x < size && y >= 0 && y < size)
        {
            return board[x][y];
        }
        return -1;
    }
    void setIn(int x, int y, int piece)
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
                if (board[x][y] == 0)
                {
                    cout << '.';
                }
                else if (board[x][y] == 1)
                {
                    cout << 'X';
                }
                else if (board[x][y] == 2)
                {
                    cout << 'O';
                }
                cout << ' '; // 棋盘点位
            }
            cout << endl;
        }
    }
};

class Referee
{
private:
    int countConsecutive(Board board, int x, int y, int dx, int dy, int piece) // 检查连子数量
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
    bool isThree(Board &board, int x, int y, int dx, int dy, int piece) // 检查是否形成活三
    {
        // 活三的两侧至少有一个空位
        return (countConsecutive(board, x, y, dx, dy, piece) == 2) &&
               (board.getIn(x - dx, y - dy) == 0 || board.getIn(x + 3 * dx, y + 3 * dy) == 0);
    }
    bool isFour(Board &board, int x, int y, int dx, int dy, int piece) // 检查是否形成活四
    {
        // 活四的两侧至少有一个空位
        return (countConsecutive(board, x, y, dx, dy, piece) == 3) &&
               (board.getIn(x - dx, y - dy) == 0 || board.getIn(x + 4 * dx, y + 4 * dy) == 0);
    }
    bool isLongLineBan(Board &board, int x, int y, int piece) // 检查长连禁手
    {
        // 长连是指超过五个连续的棋子
        return (countConsecutive(board, x, y, 0, 1, piece) + countConsecutive(board, x, y, 0, -1, piece) > 4) ||
               (countConsecutive(board, x, y, 1, 0, piece) + countConsecutive(board, x, y, -1, 0, piece) > 4) ||
               (countConsecutive(board, x, y, 1, 1, piece) + countConsecutive(board, x, y, -1, -1, piece) > 4) ||
               (countConsecutive(board, x, y, 1, -1, piece) + countConsecutive(board, x, y, -1, 1, piece) > 4);
    }

    bool isThreeThreeBan(Board &board, int x, int y, int piece) // 检查三三禁手
    {
        int threeCount = 0;
        // 检查四个方向
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0)
                    continue;
                if (isThree(board, x, y, dx, dy, piece))
                    threeCount++;
            }
        }
        return threeCount >= 2;
    }

    bool isFourFourBan(Board& board, int x, int y, int piece) // 检查四四禁手
    {
        int fourCount = 0;
        // 检查四个方向
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0)
                    continue;
                if (isFour(board, x, y, dx, dy, piece))
                    fourCount++;
            }
        }
        return fourCount >= 2;
    }
    bool checkBan(Board &board, int x, int y, int piece)
    {
        if (piece == 1)
        {
            if (
                isLongLineBan(board, x, y, piece) ||
                isThreeThreeBan(board, x, y, piece) ||
                isFourFourBan(board, x, y, piece))
            {
                return true;
            }
        }
        return false;
    }

public:
    bool checkInBoundAndEmpty(Board &board, int x, int y)
    {
        if (x >= 0 && x < board.size && y >= 0 && y < board.size && board.getIn(x, y) == 0)
        {
            return true;
        }
        return false;
    }
    bool checkAvailable(Board &board, int x, int y, int piece)
    {
        if (x >= 0 && x < board.size && y >= 0 && y < board.size && board.getIn(x, y) == 0 && !checkBan(board, x, y, piece))
        {
            return true;
        }
        return false;
    }
    bool checkWin(Board &board, int currentPiece) // 胜负判断
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

class HumanPlayer
{
private:
    int piece;
    Referee referee;

public:
    HumanPlayer() {}
    void setPiece(int choosePiece)
    {
        piece = choosePiece;
    }
    Board move(Board &board)
    {
        int x, y;
        cout << "Enter your move (row and column): " << endl;
        while (true)
        {
            // 输入
            cin >> x >> y;
            // 判别
            if (referee.checkAvailable(board, x, y, piece))
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

class AI
{
private:
    int piece;
    int opponentPiece;
    Referee referee;
    vector<pair<int, int>> getAvailableMoves(Board &board, int piece)
    {
        vector<pair<int, int>> availableMoves;
        for (int x = 0; x < board.size; x++)
        {
            for (int y = 0; y < board.size; y++)
            {
                if (referee.checkAvailable(board, x, y, piece))
                {
                    availableMoves.emplace_back(x, y);
                }
            }
        }
        return availableMoves;
    }
    int evaluateLine(Board &board, int x, int y, int dx, int dy, int piece)
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
        if (referee.checkInBoundAndEmpty(board, tx, ty))
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
        if (referee.checkInBoundAndEmpty(board, tx, ty))
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
    int evaluatePosition(Board &board, int x, int y, int piece)
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
    int evaluateBoard(Board &board, int piece)
    {
        int opponentPiece = (piece == 1) ? 2 : 1;
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
    int minimax(Board &board, int depth, int currentpiece, int alpha, int beta)
    {
        if (depth == 0)
        {
            return evaluateBoard(board, piece);
        }
        if (currentpiece == piece)
        {
            int maxEval = INT_MIN;
            for (const auto &move : getAvailableMoves(board, piece))
            {
                board.setIn(move.first, move.second, piece);
                int eval = minimax(board, depth - 1, opponentPiece, alpha, beta);
                board.setIn(move.first, move.second, 0);
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
            for (const auto &move : getAvailableMoves(board, piece))
            {
                board.setIn(move.first, move.second, opponentPiece);
                int eval = minimax(board, depth - 1, piece, alpha, beta);
                board.setIn(move.first, move.second, 0);
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
    void setPiece(int choosePiece)
    {
        piece = choosePiece;
        opponentPiece = (choosePiece == 1) ? 2 : 1;
    }
    pair<int, int> getBestPoint(Board &board, int piece, int depth)
    {
        int bestScore = INT_MIN;
        pair<int, int> bestMove;
        for (const auto &move : getAvailableMoves(board, piece))
        {
            board.setIn(move.first, move.second, piece);
            int score = minimax(board, depth, false, INT_MIN, INT_MAX);
            board.setIn(move.first, move.second, 0);

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }
};

class RobotPlayer
{
private:
    int piece;
    int opponentPiece;
    int depth;
    AI ai;

public:
    RobotPlayer() {}
    void setPiece(int choosePiece, int difficulty)
    {
        piece = choosePiece;
        opponentPiece = (choosePiece == 1) ? 2 : 1;
        depth = difficulty - 1;
        ai.setPiece(choosePiece);
    }
    Board move(Board &board)
    {
        pair<int, int> bestMove = ai.getBestPoint(board, piece, depth);
        board.setIn(bestMove.first, bestMove.second, piece);
        cout << "Robot plays at: (" << bestMove.first << ", " << bestMove.second << ")" << endl;

        return board;
    }
};

#endif