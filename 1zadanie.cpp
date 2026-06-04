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
    for (const vector<int>& row : matrix) {
        for (int value : row) {
            cout << setw(4) << value;
        }

        cout << endl;
    }
}

void firstTask() {
    int n;
    cout << "Введите четное N > 6: ";
    cin >> n;

    if (n <= 6 || n % 2 != 0) {
        cout << "Ошибка: N должно быть четным и больше 6" << endl;
        return;
    }

    vector<vector<int>> matrix(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = randomInt(0, 100);
        }
    }

    cout << endl;
    cout << "Исходная матрица:" << endl;
    printMatrix(matrix);

    int half = n / 2;
    vector<vector<int>> quarter(half, vector<int>(half));

    int sum = 0;
    int maxValue = matrix[0][0];

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            quarter[i][j] = matrix[i][j];
            sum += quarter[i][j];

            if (quarter[i][j] > maxValue) {
                maxValue = quarter[i][j];
            }
        }
    }

    cout << endl;
    cout << "Левая верхняя четверть:" << endl;
    printMatrix(quarter);

    cout << endl;
    cout << "Сумма элементов левой верхней четверти: " << sum << endl;
    cout << "Максимальный элемент в левой верхней четверти: " << maxValue << endl;
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

void secondTask() {
    int rows;
    int columns;

    cout << "Введите M > 5: ";
    cin >> rows;

    cout << "Введите N > 5: ";
    cin >> columns;

    if (rows <= 5 || columns <= 5) {
        cout << "Ошибка: M и N должны быть больше 5" << endl;
        return;
    }

    vector<vector<int>> matrix(rows, vector<int>(columns));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = randomInt(1000, 5000);
        }
    }

    cout << endl;
    cout << "Исходная матрица:" << endl;
    printMatrix(matrix);

    vector<int> rowSums(rows);
    int minRowIndex = 0;

    cout << endl;
    cout << "Таблица сумм цифр:" << endl;
    cout << "Номер строки | Сумма цифр" << endl;

    for (int i = 0; i < rows; i++) {
        int rowSum = 0;

        for (int j = 0; j < columns; j++) {
            rowSum += sumDigits(matrix[i][j]);
        }

        rowSums[i] = rowSum;

        cout << setw(12) << i << " | " << rowSum << endl;

        if (rowSums[i] < rowSums[minRowIndex]) {
            minRowIndex = i;
        }
    }

    int* resultArray = new int[columns];

    for (int j = 0; j < columns; j++) {
        resultArray[j] = matrix[minRowIndex][j];
    }

    cout << endl;
    cout << "Строка с наименьшей суммой цифр: " << minRowIndex << endl;
    cout << "Сумма цифр этой строки: " << rowSums[minRowIndex] << endl;

    cout << "Динамический массив из элементов найденной строки:" << endl;
    for (int j = 0; j < columns; j++) {
        cout << resultArray[j] << ' ';
    }
    cout << endl;

    delete[] resultArray;
}

void clearConsole() {
    system("clear");
}

void printLifeBoard(const vector<vector<int>>& board) {
    for (const vector<int>& row : board) {
        for (int cell : row) {
            if (cell == 1) {
                cout << '#';
            } else {
                cout << ' ';
            }
        }

        cout << endl;
    }
}

int countAliveNeighbours(const vector<vector<int>>& board, int row, int column) {
    int rows = board.size();
    int columns = board[0].size();
    int count = 0;

    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        for (int columnOffset = -1; columnOffset <= 1; columnOffset++) {
            if (rowOffset == 0 && columnOffset == 0) {
                continue;
            }

            int nextRow = row + rowOffset;
            int nextColumn = column + columnOffset;

            if (nextRow >= 0 && nextRow < rows && nextColumn >= 0 && nextColumn < columns) {
                count += board[nextRow][nextColumn];
            }
        }
    }

    return count;
}

vector<vector<int>> createNextGeneration(const vector<vector<int>>& board) {
    int rows = board.size();
    int columns = board[0].size();

    vector<vector<int>> nextBoard(rows, vector<int>(columns, 0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int neighbours = countAliveNeighbours(board, i, j);

            if (board[i][j] == 1) {
                if (neighbours == 2 || neighbours == 3) {
                    nextBoard[i][j] = 1;
                }
            } else {
                if (neighbours == 3) {
                    nextBoard[i][j] = 1;
                }
            }
        }
    }

    return nextBoard;
}

void placeEater(vector<vector<int>>& board, int row, int column) {
    int rows = board.size();
    int columns = board[0].size();

    if (row + 3 >= rows || column + 2 >= columns) {
        return;
    }

    board[row][column] = 1;
    board[row][column + 1] = 1;
    board[row + 1][column] = 1;
    board[row + 2][column + 1] = 1;
    board[row + 3][column + 1] = 1;
    board[row + 3][column + 2] = 1;
}

void randomFill(vector<vector<int>>& board, int percent) {
    for (vector<int>& row : board) {
        for (int& cell : row) {
            if (randomInt(1, 100) <= percent) {
                cell = 1;
            } else {
                cell = 0;
            }
        }
    }
}

void runLife(vector<vector<int>> board, int generations, int delayMs) {
    for (int generation = 0; generation < generations; generation++) {
        clearConsole();

        cout << "Поколение: " << generation << endl;
        cout << endl;

        printLifeBoard(board);

        board = createNextGeneration(board);

        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

void thirdTask() {
    int choice;

    cout << "1 - показать фигуру Eater" << endl;
    cout << "2 - случайная колония" << endl;
    cout << "Выбор: ";
    cin >> choice;

    int rows = 25;
    int columns = 60;

    vector<vector<int>> board(rows, vector<int>(columns, 0));

    if (choice == 1) {
        placeEater(board, 10, 25);
    } else {
        randomFill(board, 25);
    }

    runLife(board, 100, 250);
}

int main() {
    int choice;

    cout << "1 - левая верхняя четверть квадратной матрицы" << endl;
    cout << "2 - сумма цифр элементов строк" << endl;
    cout << "3 - клеточный автомат Джона Конвея" << endl;
    cout << "Выбор: ";
    cin >> choice;

    if (choice == 1) {
        firstTask();
    } else if (choice == 2) {
        secondTask();
    } else if (choice == 3) {
        thirdTask();
    } else {
        cout << "Неверный выбор" << endl;
    }

    return 0;
}