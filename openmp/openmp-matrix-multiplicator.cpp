// This is the application entry point.

#include "openmp-matrix-multiplicator.h"

using namespace std;

// TODO: matrix -> struct
// TODO: some exceptions for incorrect file/matrix format

void parseFromCsv(string path) {
}

void generateRandom(int minValue, int maxValue, int r, int c) {
	
}

void multiply(vector<vector<int>> m1, vector<vector<int>> m2, int r1, int r2, int c1, int c2) {
	#ifdef _OPENMP
	vector<vector<int>> result(r1, vector<int>(c2));
	for (int i = 0; i < r1; ++i) {
		for (int j = 0; j < c2; ++j) {
			int tmp = 0;
			for (int k = 0; k < c1; ++k) {
				tmp += m1[i][k] * m2[k][j];
			}
			result[i][j] = tmp;
		}
	}

	for (int i = 0; i < r1; ++i) {
		for (int j = 0; j < c2; ++j) {
			cout << result[i][j] << " ";
		}
		cout << endl;
	}
	#endif
}

int main()

{
	int r1, r2, c1, c2;
	cin >> r1 >> c1 >> r2 >> c2;
	vector<vector<int>> matrix1(r1, vector<int>(c1));
	vector<vector<int>> matrix2(r2, vector<int>(c2));

	for (int i = 0; i < r1; i++) {
		for (int j = 0; j < c1; j++) {
			cin >> matrix1[i][j];
		}
	}
	for (int i = 0; i < r2; i++) {
		for (int j = 0; j < c2; j++) {
			cin >> matrix2[i][j];
		}
	}

	/*for (int i = 0; i < r1; i++) {
		for (int j = 0; j < c1; j++) {
			cout << matrix1[i][j] << " ";
		}
		cout << endl;
	}*/

	multiply(matrix1, matrix2, r1, r2, c1, c2);

	
	cout << "Hello CMake." << endl;
	return 0;
	
}



