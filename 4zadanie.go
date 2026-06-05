package main

import (
	"fmt"
)

type Pair struct {
	first  int
	second int
}

func boardToMask(board []int) int {
	mask := 0

	for i := 0; i < len(board); i++ {
		if board[i] == 1 {
			mask = mask | (1 << i)
		}
	}

	return mask
}

func createChessBoardMask(rows int, columns int, firstValue int) int {
	board := make([]int, rows*columns)

	for i := 0; i < rows; i++ {
		for j := 0; j < columns; j++ {
			if (i+j)%2 == 0 {
				board[i*columns+j] = firstValue
			} else {
				board[i*columns+j] = 1 - firstValue
			}
		}
	}

	return boardToMask(board)
}

func createNeighborPairs(rows int, columns int) []Pair {
	pairs := make([]Pair, 0)

	for i := 0; i < rows; i++ {
		for j := 0; j < columns; j++ {
			current := i*columns + j

			if i+1 < rows {
				pairs = append(pairs, Pair{current, (i+1)*columns + j})
			}

			if j+1 < columns {
				pairs = append(pairs, Pair{current, i*columns + j + 1})
			}
		}
	}

	return pairs
}

func findMinMoves(start int, firstTarget int, secondTarget int, rows int, columns int) int {
	if start == firstTarget || start == secondTarget {
		return 0
	}

	pairs := createNeighborPairs(rows, columns)

	queueValues := make([]int, 0)
	distance := make(map[int]int)

	queueValues = append(queueValues, start)
	distance[start] = 0

	for len(queueValues) > 0 {
		current := queueValues[0]
		queueValues = queueValues[1:]

		for _, currentPair := range pairs {
			next := current

			next = next ^ (1 << currentPair.first)
			next = next ^ (1 << currentPair.second)

			if _, exists := distance[next]; !exists {
				distance[next] = distance[current] + 1

				if next == firstTarget || next == secondTarget {
					return distance[next]
				}

				queueValues = append(queueValues, next)
			}
		}
	}

	return -1
}

func printBoard(board []int, rows int, columns int) {
	for i := 0; i < rows; i++ {
		for j := 0; j < columns; j++ {
			fmt.Printf("%d ", board[i*columns+j])
		}

		fmt.Println()
	}
}

func main() {
	var rows int
	var columns int

	fmt.Print("Введите количество строк: ")
	fmt.Scan(&rows)

	fmt.Print("Введите количество столбцов: ")
	fmt.Scan(&columns)

	if rows <= 0 || columns <= 0 {
		fmt.Println("Ошибка: размеры должны быть положительными")
		return
	}

	if rows*columns > 25 {
		fmt.Println("Ошибка: для полного перебора размер ограничен 25 клетками")
		fmt.Println("Введите меньшую доску, например 2x2, 2x3, 3x3 или 4x4")
		return
	}

	board := make([]int, rows*columns)

	fmt.Println("Введите доску из 0 и 1:")

	for i := 0; i < rows*columns; i++ {
		fmt.Scan(&board[i])

		if board[i] != 0 && board[i] != 1 {
			fmt.Println("Ошибка: можно вводить только 0 или 1")
			return
		}
	}

	start := boardToMask(board)
	firstTarget := createChessBoardMask(rows, columns, 0)
	secondTarget := createChessBoardMask(rows, columns, 1)

	fmt.Println()
	fmt.Println("Исходная доска:")
	printBoard(board, rows, columns)

	answer := findMinMoves(start, firstTarget, secondTarget, rows, columns)

	fmt.Println()

	if answer == -1 {
		fmt.Println("Получить шахматный паттерн невозможно")
	} else {
		fmt.Println("Минимальное количество ходов:", answer)
	}
}