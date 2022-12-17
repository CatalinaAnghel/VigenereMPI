#include "matrix_helper.h"

void MatrixHelper::initializeMatrix(char matrix[][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int column = 0; column < SIZE; column++) {
			matrix[row][column] = 'A' + row + column;
			if ('A' + row + column > 'Z') {
				matrix[row][column] = matrix[row][column] - 'Z' - 1 + 'A';
			}
		}
	}
}


void MatrixHelper::printMatrix(char matrix[][SIZE]) {
	for (int row = 0; row < SIZE; row++) {
		for (int column = 0; column < SIZE; column++) {
			cout << matrix[row][column] << " ";
		}
		cout << endl;
	}
}