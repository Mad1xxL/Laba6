#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
#include <thread>
#include <vector>

using namespace std;

int randomInt(int left, int right) {
    static mt19937 generator(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<int> distribution(left, right);
    return distribution(generator);
}

void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int value : row) {
            cout << setw(4) << value;
        }
        cout << '\n';
    }
}

void task1_1() {
    int n;
    cout << "Введите четное N > 6: ";
    cin >> n;

    if (n <= 6 || n % 2 != 0) {
        cout << "Ошибка: N должно быть четным и больше 6.\n";
        return;
    }

    vector<vector<int>> matrix(n, vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = randomInt(0, 100);
        }
    }

    cout << "\nИсходная матрица:\n";
    printMatrix(matrix);

    int half = n / 2;
    vector<vector<int>> quarter(half, vector<int>(half));

    int sum = 0;
    int maxValue = matrix[0][0];

    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            quarter[i][j] = matrix[i][j];
            sum += quarter[i][j];

            if (quarter[i][j] > maxValue) {
                maxValue = quarter[i][j];
            }
        }
    }

    cout << "\nЛевая верхняя четверть:\n";
    printMatrix(quarter);

    cout << "\nСумма элементов левой верхней четверти: " << sum << '\n';
    cout << "Максимальный элемент в левой верхней четверти: " << maxValue << '\n';
}

int sumDigits(int number) {
    number = abs(number);
    int sum = 0;

    while (number > 0) {
        sum += number % 10;
        number /= 10;
    }

    return sum;
}

void task1_2() {
    int m, n;
    cout << "Введите M > 5 и N > 5: ";
    cin >> m >> n;

    if (m <= 5 || n <= 5) {
        cout << "Ошибка: M и N должны быть больше 5.\n";
        return;
    }

    vector<vector<int>> matrix(m, vector<int>(n));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = randomInt(1000, 5000);
        }
    }

    cout << "\nИсходная матрица:\n";
    printMatrix(matrix);

    vector<int> rowDigitSums(m);
    int minRowIndex = 0;

    cout << "\nТаблица сумм цифр:\n";
    cout << "Номер строки | Сумма цифр\n";

    for (int i = 0; i < m; ++i) {
        int rowSum = 0;

        for (int j = 0; j < n; ++j) {
            rowSum += sumDigits(matrix[i][j]);
        }

        rowDigitSums[i] = rowSum;

        cout << setw(12) << i << " | " << rowSum << '\n';

        if (rowDigitSums[i] < rowDigitSums[minRowIndex]) {
            minRowIndex = i;
        }
    }

    int* dynamicArray = new int[n];

    for (int j = 0; j < n; ++j) {
        dynamicArray[j] = matrix[minRowIndex][j];
    }

    cout << "\nСтрока с наименьшей суммой цифр: " << minRowIndex << '\n';
    cout << "Сумма цифр этой строки: " << rowDigitSums[minRowIndex] << '\n';

    cout << "Динамический массив из элементов найденной строки:\n";
    for (int j = 0; j < n; ++j) {
        cout << dynamicArray[j] << ' ';
    }
    cout << '\n';

    delete[] dynamicArray;
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLifeBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            cout << (cell ? '#' : ' ');
        }
        cout << '\n';
    }
}

int countAliveNeighbours(const vector<vector<int>>& board, int row, int col) {
    int rows = static_cast<int>(board.size());
    int cols = static_cast<int>(board[0].size());
    int count = 0;

    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) {
                continue;
            }

            int ni = row + di;
            int nj = col + dj;

            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                count += board[ni][nj];
            }
        }
    }

    return count;
}

vector<vector<int>> nextGeneration(const vector<vector<int>>& board) {
    int rows = static_cast<int>(board.size());
    int cols = static_cast<int>(board[0].size());

    vector<vector<int>> next(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int neighbours = countAliveNeighbours(board, i, j);

            if (board[i][j] == 1) {
                next[i][j] = (neighbours == 2 || neighbours == 3);
            } else {
                next[i][j] = (neighbours == 3);
            }
        }
    }

    return next;
}

void placeEater(vector<vector<int>>& board, int row, int col) {
    vector<pair<int, int>> cells = {
        {0, 0}, {0, 1},
        {1, 0},
        {2, 1},
        {3, 1}, {3, 2}
    };

    for (auto [di, dj] : cells) {
        int r = row + di;
        int c = col + dj;

        if (r >= 0 && r < static_cast<int>(board.size()) &&
            c >= 0 && c < static_cast<int>(board[0].size())) {
            board[r][c] = 1;
        }
    }
}

void randomFill(vector<vector<int>>& board, int percent) {
    for (auto& row : board) {
        for (int& cell : row) {
            cell = randomInt(1, 100) <= percent ? 1 : 0;
        }
    }
}

void runLife(vector<vector<int>> board, int generations, int delayMs) {
    for (int generation = 0; generation < generations; ++generation) {
        clearConsole();
        cout << "Поколение: " << generation << "\n\n";
        printLifeBoard(board);
        board = nextGeneration(board);
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

void task1_3() {
    int choice;
    cout << "1 - показать фигуру Eater\n";
    cout << "2 - случайная колония\n";
    cout << "Выбор: ";
    cin >> choice;

    const int rows = 25;
    const int cols = 60;

    vector<vector<int>> board(rows, vector<int>(cols, 0));

    if (choice == 1) {
        placeEater(board, 10, 25);
    } else {
        randomFill(board, 25);
    }

    runLife(board, 100, 250);
}

int main() {
    setlocale(LC_ALL, "Russian");

    int choice;

    cout << "1 - левая верхняя четверть квадратной матрицы\n";
    cout << "2 - сумма цифр элементов строк\n";
    cout << "3 - клеточный автомат Джона Конвея\n";
    cout << "Выбор: ";
    cin >> choice;

    if (choice == 1) {
        task1_1();
    } else if (choice == 2) {
        task1_2();
    } else if (choice == 3) {
        task1_3();
    } else {
        cout << "Неверный выбор.\n";
    }

    return 0;
}