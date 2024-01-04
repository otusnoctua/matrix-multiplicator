// This is the application entry point.

#include "openmp-matrix-multiplicator.h"

using namespace std;

int checkMultiCriteria(int r2, int c1) {
	if (r2 != c1) {
		cout << "Error: multiplication is not possible (rows(m2) != columns(m1))!";
	}
	return 0;
}

void generateRandomMatrices(int **matrix, int r, int c) {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			matrix[i][j] = rand() % 11;
		}
	}
	return;
}

void multiply(int **m1, int **m2, int **result, int r1, int r2, int c1, int c2) {
	// Threads number is defined here
	int threads = 2;
	omp_set_num_threads(threads);
	int i, j, k;
#pragma omp parallel for shared(m1, m2, result) private(i, j, k)
	for (int i = 0; i < r1; ++i) {
		for (int j = 0; j < c2; ++j) {
			int tmp = 0;
			for (int k = 0; k < c1; ++k) {
				tmp += m1[i][k] * m2[k][j];
			}
			result[i][j] = tmp;
		}
	}
	return;
}

int main() {
	srand(time(NULL));

	int r1 = 1500;
	int c1 = 750;

	int r2 = 750;
	int c2 = 1000;

	checkMultiCriteria(r2, c1);

	// Matrices initialization
	int** matrix1;
	int** matrix2;
	int** result;

	// Initial matrices memory allocation
	matrix1 = (int**)malloc(sizeof(int*) * r1);
	for (int i = 0; i < r1; i++) {
		matrix1[i] = (int*)malloc(sizeof(int) * c1);
	}
	matrix2 = (int**)malloc(sizeof(int*) * r2);
	for (int i = 0; i < r2; i++) {
		matrix2[i] = (int*)malloc(sizeof(int) * c2);
	}

	// Result matrix memory allocation
	result = (int**)malloc(sizeof(int*) * r1);
	for (int i = 0; i < r1; i++) {
		result[i] = (int*)malloc(sizeof(int) * c2);
	}

	generateRandomMatrices(matrix1, r1, c1);
	generateRandomMatrices(matrix2, r2, c2);
	
	clock_t start = clock();
	multiply(matrix1, matrix2, result, r1, r2, c1, c2);
	clock_t end = clock();

	printf("Multiplication time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

	return 0;
}