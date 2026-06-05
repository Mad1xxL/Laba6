package main

import (
	"fmt"
	"math"
)

type Matrix [][]float64
type Vector []float64

func printMatrix(title string, matrix Matrix) {
	fmt.Println(title)

	for _, row := range matrix {
		for _, value := range row {
			fmt.Printf("%12.6f", value)
		}
		fmt.Println()
	}

	fmt.Println()
}

func printVector(title string, values Vector) {
	fmt.Println(title)

	for i, value := range values {
		fmt.Printf("x%d = %.6f\n", i+1, value)
	}

	fmt.Println()
}

func getDifference(first Vector, second Vector) float64 {
	maxDifference := 0.0

	for i := 0; i < len(first); i++ {
		difference := math.Abs(first[i] - second[i])

	 if difference > maxDifference {
			maxDifference = difference
		}
	}

	return maxDifference
}

func solveLU(matrix Matrix, rightPart Vector) Vector {
	size := len(matrix)

	lowerMatrix := make(Matrix, size)
	upperMatrix := make(Matrix, size)

	for i := 0; i < size; i++ {
		lowerMatrix[i] = make(Vector, size)
		upperMatrix[i] = make(Vector, size)
		lowerMatrix[i][i] = 1
	}

	for k := 0; k < size; k++ {
		for j := k; j < size; j++ {
			sum := 0.0

			for s := 0; s < k; s++ {
				sum += lowerMatrix[k][s] * upperMatrix[s][j]
			}

			upperMatrix[k][j] = matrix[k][j] - sum
		}

		if math.Abs(upperMatrix[k][k]) < 1e-12 {
			fmt.Println("Ошибка: нулевой ведущий элемент. LU-разложение невозможно.")
			return Vector{}
		}

		for i := k + 1; i < size; i++ {
			sum := 0.0

			for s := 0; s < k; s++ {
				sum += lowerMatrix[i][s] * upperMatrix[s][k]
			}

			lowerMatrix[i][k] = (matrix[i][k] - sum) / upperMatrix[k][k]
		}
	}

	printMatrix("Матрица L:", lowerMatrix)
	printMatrix("Матрица U:", upperMatrix)

	y := make(Vector, size)

	for i := 0; i < size; i++ {
		sum := 0.0

		for j := 0; j < i; j++ {
			sum += lowerMatrix[i][j] * y[j]
		}

		y[i] = rightPart[i] - sum
	}

	printVector("Решение системы L * y = b:", y)

	x := make(Vector, size)

	for i := size - 1; i >= 0; i-- {
		sum := 0.0

		for j := i + 1; j < size; j++ {
			sum += upperMatrix[i][j] * x[j]
		}

		x[i] = (y[i] - sum) / upperMatrix[i][i]
	}

	return x
}

func solveBySeidel(matrix Matrix, rightPart Vector, epsilon float64) Vector {
	size := len(matrix)

	x := make(Vector, size)
	previousX := make(Vector, size)

	fmt.Println("Итерационный метод Зейделя:")
	fmt.Printf("%s%14s%14s%14s%14s%14s\n", "N", "x1", "x2", "x3", "x4", "eps")

	for iteration := 1; iteration <= 10000; iteration++ {
		copy(previousX, x)

		for i := 0; i < size; i++ {
			sum := rightPart[i]

			for j := 0; j < size; j++ {
				if j != i {
					sum -= matrix[i][j] * x[j]
				}
			}

			x[i] = sum / matrix[i][i]
		}

		difference := getDifference(x, previousX)

		fmt.Printf(
			"%d%14.6f%14.6f%14.6f%14.6f%14.6f\n",
			iteration,
			x[0],
			x[1],
			x[2],
			x[3],
			difference,
		)

		if difference < epsilon {
			fmt.Println()
			return x
		}
	}

	fmt.Println()
	fmt.Println("Достигнуто максимальное количество итераций.")
	return x
}

func main() {
	epsilon := 0.001

	m := -1.21
	n := 0.2
	p := 0.88

	matrix := Matrix{
		{m, -0.04, 0.21, -1.16},
		{0.25, -1.23, n, -0.09},
		{-0.21, n, 0.8, -0.13},
		{0.15, -1.31, 0.06, p},
	}

	rightPart := Vector{-1.24, p, 2.56, m}

	fmt.Printf("M = %.2f, N = %.1f, P = %.2f\n", m, n, p)
	fmt.Printf("Точность = %.3f\n\n", epsilon)

	printMatrix("Матрица A:", matrix)
	printVector("Вектор b:", rightPart)

	luAnswer := solveLU(matrix, rightPart)
	printVector("Решение прямым методом LU-разложения:", luAnswer)

	seidelAnswer := solveBySeidel(matrix, rightPart, epsilon)
	printVector("Решение итерационным методом Зейделя:", seidelAnswer)
}