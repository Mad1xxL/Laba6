#include <iostream>
#include <queue>
#include <map>
#include <vector>

using namespace std;

// Переводит доску из массива 0 и 1 в битовую маску.
int boardToMask(const vector<int>& board) {
    int mask = 0;

    for (int i = 0; i < board.size(); i++) {
        if (board[i] == 1) {
            mask = mask | (1 << i);
        }
    }

    return mask;
}

// Создает целевую шахматную доску и переводит ее в битовую маску.
int createChessBoardMask(int rows, int columns, int firstValue) {
    vector<int> board(rows * columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if ((i + j) % 2 == 0) {
                board[i * columns + j] = firstValue;
            } else {
                board[i * columns + j] = 1 - firstValue;
            }
        }
    }

    return boardToMask(board);
}

// Создает список всех соседних пар клеток.
vector<pair<int, int>> createNeighborPairs(int rows, int columns) {
    vector<pair<int, int>> pairs;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int current = i * columns + j;

            if (i + 1 < rows) {
                pairs.push_back({current, (i + 1) * columns + j});
            }

            if (j + 1 < columns) {
                pairs.push_back({current, i * columns + j + 1});
            }
        }
    }

    return pairs;
}

// Поиск минимальных шагов 
int findMinMoves(int start, int firstTarget, int secondTarget, int rows, int columns) {
    if (start == firstTarget || start == secondTarget) {
        return 0;
    }

    vector<pair<int, int>> pairs = createNeighborPairs(rows, columns);

    queue<int> queueValues;
    map<int, int> distance;

    queueValues.push(start);
    distance[start] = 0;

    while (!queueValues.empty()) {
        int current = queueValues.front();
        queueValues.pop();

        for (pair<int, int> currentPair : pairs) {
            int next = current;

            next = next ^ (1 << currentPair.first);
            next = next ^ (1 << currentPair.second);

            if (distance.count(next) == 0) {
                distance[next] = distance[current] + 1;

                if (next == firstTarget || next == secondTarget) {
                    return distance[next];
                }

                queueValues.push(next);
            }
        }
    }

    return -1;
}

void printBoard(const vector<int>& board, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << board[i * columns + j] << ' ';
        }

        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    int rows;
    int columns;

    cout << "Введите количество строк: ";
    cin >> rows;

    cout << "Введите количество столбцов: ";
    cin >> columns;

    if (rows <= 0 || columns <= 0) {
        cout << "Ошибка: размеры должны быть положительными" << endl;
        return 1;
    }

    if (rows * columns > 25) {
        cout << "Ошибка: для полного перебора размер ограничен 25 клетками" << endl;
        cout << "Введите меньшую доску, например 2x2, 2x3, 3x3 или 4x4" << endl;
        return 1;
    }

    vector<int> board(rows * columns);

    cout << "Введите доску из 0 и 1:" << endl;

    for (int i = 0; i < rows * columns; i++) {
        cin >> board[i];

        if (board[i] != 0 && board[i] != 1) {
            cout << "Ошибка: можно вводить только 0 или 1" << endl;
            return 1;
        }
    }

    int start = boardToMask(board);
    int firstTarget = createChessBoardMask(rows, columns, 0);
    int secondTarget = createChessBoardMask(rows, columns, 1);

    cout << endl;
    cout << "Исходная доска:" << endl;
    printBoard(board, rows, columns);

    int answer = findMinMoves(start, firstTarget, secondTarget, rows, columns);

    cout << endl;

    if (answer == -1) {
        cout << "Получить шахматный паттерн невозможно" << endl;
    } else {
        cout << "Минимальное количество ходов: " << answer << endl;
    }

    return 0;
}