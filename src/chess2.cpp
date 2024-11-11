#include <iostream>
#include <vector>
#include <algorithm>
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

class Game
{
private:
    Board board;
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
    pair<int, int> humanMove()
    {
        int x, y;
        cout << "Player " << currentPlayer << ", enter your move (row and column): " << endl;
        while (true)
        {
            // 输入
            cin >> x >> y;
            // 判别
            if (board.isInboundsAndEmpty(x, y))
            {
                board.setIn(x, y, currentPlayer);
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
    int evaluatePosition(int x, int y, char piece)
    {
        int score = 0;
        // 检查所有方向
        score += board.size - max(abs(x - board.size/2), abs(y - board.size/2)) * 2;
        score += evaluateLine(x, y, 0, 1, piece);  // 横向
        score += evaluateLine(x, y, 1, 0, piece);  // 纵向
        score += evaluateLine(x, y, 1, 1, piece);  // 主对角线
        score += evaluateLine(x, y, 1, -1, piece); // 副对角线
        return score;
    }
    int evaluateBoard(char player) {
        int score = 0;
        int opponentScore = 0;
        // 遍历棋盘，评估每个位置
        for (int i = 0; i < board.size; ++i) {
            for (int j = 0; j < board.size; ++j) {
                if (board.getIn(i, j) == player) {
                    score += evaluatePosition(i, j, player);
                }
                else if (board.getIn(i, j) == (player == 'X') ? 'O' : 'X') {
                    opponentScore += evaluatePosition(i, j, (player == 'X') ? 'O' : 'X');
                }
            }
        }
        // 返回当前玩家的得分减去对手的得分
        return score - opponentScore;
    }
    int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta)
    {
        if (depth == 0 || checkWin(currentPlayer))
        {
            return evaluateBoard(currentPlayer);
        }

        if (isMaximizingPlayer)
        {
            int maxEval = INT_MIN;
            for (const auto &move : board.getAvailableMoves())
            {
                board.setIn(move.first, move.second, currentPlayer);
                int eval = minimax(depth - 1, false, alpha, beta);
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
                board.setIn(move.first, move.second, (currentPlayer == 'X') ? 'O' : 'X');
                int eval = minimax(depth - 1, true, alpha, beta);
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
    pair<int, int> robotMove()
    {
        int bestScore = INT_MIN;
        pair<int, int> bestMove;
        int depth = 1; // 定义搜索深度

        for (const auto &move : board.getAvailableMoves())
        {
            board.setIn(move.first, move.second, currentPlayer);
            int score = minimax(depth, false, INT_MIN, INT_MAX);
            board.setIn(move.first, move.second, '.');

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }

        board.setIn(bestMove.first, bestMove.second, currentPlayer);
        cout << "Robot plays at: (" << bestMove.first << ", " << bestMove.second << ")" << endl;
        return bestMove;
    }
    bool checkWin(char player) // 胜负判断
    {
        for (int i = 0; i < board.size; ++i) {
            for (int j = 0; j < board.size; ++j) {
                if (board.getIn(i, j) == player && evaluatePosition(i, j, player) >= 100000) {
                    return true;
                }
            }
        }
        return false;
    };

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
            if (checkWin(currentPlayer))
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