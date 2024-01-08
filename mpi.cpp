#include <iostream>
#include <mpi.h>

using namespace std;

// Matrix multiplication by stripes requires matrix size N * N
#define N 1000

void generateRandomMatrices(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i] = rand() % 11;
        }
    }
    return;
}

int main(int argc, char** argv) {
    int rank, size, spareData, stripSize;
    int* matrix1 = new int[N * N], * matrix2 = new int[N * N], * result = new int[N * N];

    double start, end;

    generateRandomMatrices(matrix1, N);
    generateRandomMatrices(matrix2, N);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    stripSize = N / size;
    spareData = (rank == size - 1 && N % size != 0) ? N % size : 0;

    if (rank == 0) {

        start = MPI_Wtime();

        for (int i = 1; i < size; ++i) {
            MPI_Send(
                &matrix1[stripSize * i * N],
                (i == size - 1) ? (stripSize + N % size) * N : stripSize * N,
                MPI_DOUBLE,
                i,
                1,
                MPI_COMM_WORLD
            );
            MPI_Send(
                &matrix2[0],
                N * N,
                MPI_DOUBLE,
                i,
                2,
                MPI_COMM_WORLD
            );
        }
    }
    else {
        MPI_Recv(
            &matrix1[stripSize * rank * N],
            (stripSize + spareData) * N,
            MPI_DOUBLE,
            0,
            1,
            MPI_COMM_WORLD,
            &status
        );
        MPI_Recv(
            &matrix2[0],
            N * N,
            MPI_DOUBLE,
            0,
            2,
            MPI_COMM_WORLD,
            &status
        );
    }

    for (int i = stripSize * rank; i < stripSize * (rank + 1) + spareData; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i * N + j] = 0;
            for (int p = 0; p < N; ++p) {
                result[i * N + j] += matrix1[i * N + p] * matrix2[p * N + j];
            }
        }
    }

    delete[] matrix1;
    delete[] matrix2;

    if (rank != 0) {
        MPI_Send(
            &result[stripSize * rank * N], 
            (stripSize + spareData) * N, 
            MPI_DOUBLE, 
            0, 
            4, 
            MPI_COMM_WORLD
        );
    }
    else 
    {
        for (int i = 1; i < size; ++i) {
            MPI_Recv(
                &result[stripSize * i * N], 
                (i == size - 1) ? (stripSize + N % size) * N : stripSize * N, 
                MPI_DOUBLE, 
                i, 
                4, 
                MPI_COMM_WORLD, 
                &status
            );
        }

        double end = MPI_Wtime();
        printf("Multiplication time: %f\n", (double)(end - start));
    }

    MPI_Finalize();
    delete[] result;
    return 0;
}