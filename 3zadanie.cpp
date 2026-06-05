#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

using Matrix = vector<vector<double>>;
using Vector = vector<double>;

void printMatrix(const string& title, const Matrix& matrix) {
    cout << title << endl;

    for (const Vector& row : matrix) {
        for (double value : row) {
            cout << setw(12) << fixed << setprecision(6) << value;
        }
        cout << endl;
    }

    cout << endl;
}

void printVector(const string& title, const Vector& values) {
    cout << title << endl;

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << values[i] << endl;
    }

    cout << endl;
}

double getDifference(const Vector& first, const Vector& second) {
    double maxDifference = 0;

    for (int i = 0; i < static_cast<int>(first.size()); i++) {
        maxDifference = max(maxDifference, fabs(first[i] - second[i]));
    }

    return maxDifference;
}

// Решает систему A * x = b прямым методом LU-разложения.
Vector solveLU(const Matrix& matrix, const Vector& rightPart) {
    int size = static_cast<int>(matrix.size());

    Matrix lowerMatrix(size, Vector(size, 0));
    Matrix upperMatrix(size, Vector(size, 0));

    for (int i = 0; i < size; i++) {
        lowerMatrix[i][i] = 1;
    }

    for (int k = 0; k < size; k++) {
        for (int j = k; j < size; j++) {
            double sum = 0;

            for (int s = 0; s < k; s++) {
                sum += lowerMatrix[k][s] * upperMatrix[s][j];
            }

            upperMatrix[k][j] = matrix[k][j] - sum;
        }

        if (fabs(upperMatrix[k][k]) < 1e-12) {
            cout << "Ошибка: нулевой ведущий элемент. LU-разложение невозможно." << endl;
            return {};
        }

        for (int i = k + 1; i < size; i++) {
            double sum = 0;

            for (int s = 0; s < k; s++) {
                sum += lowerMatrix[i][s] * upperMatrix[s][k];
            }

            lowerMatrix[i][k] = (matrix[i][k] - sum) / upperMatrix[k][k];
        }
    }

    printMatrix("Матрица L:", lowerMatrix);
    printMatrix("Матрица U:", upperMatrix);

    Vector y(size);

    for (int i = 0; i < size; i++) {
        double sum = 0;

        for (int j = 0; j < i; j++) {
            sum += lowerMatrix[i][j] * y[j];
        }

        y[i] = rightPart[i] - sum;
    }

    printVector("Решение системы L * y = b:", y);

    Vector x(size);

    for (int i = size - 1; i >= 0; i--) {
        double sum = 0;

        for (int j = i + 1; j < size; j++) {
            sum += upperMatrix[i][j] * x[j];
        }

        x[i] = (y[i] - sum) / upperMatrix[i][i];
    }

    return x;
}

// Решает систему A * x = b итерационным методом Зейделя.
Vector solveBySeidel(const Matrix& matrix, const Vector& rightPart, double epsilon) {
    int size = static_cast<int>(matrix.size());

    Vector x(size, 0);
    Vector previousX(size, 0);

    cout << "Итерационный метод Зейделя:" << endl;
    cout << "N"
         << setw(14) << "x1"
         << setw(14) << "x2"
         << setw(14) << "x3"
         << setw(14) << "x4"
         << setw(14) << "eps" << endl;

    for (int iteration = 1; iteration <= 10000; iteration++) {
        previousX = x;

        for (int i = 0; i < size; i++) {
            double sum = rightPart[i];

            for (int j = 0; j < size; j++) {
                if (j != i) {
                    sum -= matrix[i][j] * x[j];
                }
            }

            x[i] = sum / matrix[i][i];
        }

        double difference = getDifference(x, previousX);

        cout << iteration
             << setw(14) << fixed << setprecision(6) << x[0]
             << setw(14) << x[1]
             << setw(14) << x[2]
             << setw(14) << x[3]
             << setw(14) << difference << endl;

        if (difference < epsilon) {
            cout << endl;
            return x;
        }
    }

    cout << endl;
    cout << "Достигнуто максимальное количество итераций." << endl;
    return x;
}

int main() {
    const double epsilon = 0.001;

    double m = -1.21;
    double n = 0.2;
    double p = 0.88;

    Matrix matrix = {
        {m, -0.04, 0.21, -1.16},
        {0.25, -1.23, n, -0.09},
        {-0.21, n, 0.8, -0.13},
        {0.15, -1.31, 0.06, p}
    };

    Vector rightPart = {-1.24, p, 2.56, m};

    cout << "M = " << m << ", N = " << n << ", P = " << p << endl;
    cout << "Точность = " << epsilon << endl << endl;

    printMatrix("Матрица A:", matrix);
    printVector("Вектор b:", rightPart);

    Vector luAnswer = solveLU(matrix, rightPart);
    printVector("Решение прямым методом LU-разложения:", luAnswer);

    Vector seidelAnswer = solveBySeidel(matrix, rightPart, epsilon);
    printVector("Решение итерационным методом Зейделя:", seidelAnswer);

    return 0;
}