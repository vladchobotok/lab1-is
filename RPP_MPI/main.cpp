#include <iostream>
#include <mpi.h>

using namespace std;

const int MATRIX_SIZE = 10;
int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int matrixC[MATRIX_SIZE][MATRIX_SIZE];

void generateMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    double start_time, end_time;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        generateMatrix(matrixA);
        generateMatrix(matrixB);
    }

    MPI_Bcast(matrixA, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrixB, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    int chunk_size = MATRIX_SIZE / size;
    int start_row = rank * chunk_size;
    int end_row = (rank + 1) * chunk_size;
    int* local_result = new int[chunk_size * MATRIX_SIZE];

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            local_result[(i - start_row) * MATRIX_SIZE + j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                local_result[(i - start_row) * MATRIX_SIZE + j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    MPI_Gather(local_result, chunk_size * MATRIX_SIZE, MPI_INT, matrixC, chunk_size * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    delete[] local_result;

    end_time = MPI_Wtime();

    if (rank == 0) {
        cout << "Matrix A:" << endl;
        printMatrix(matrixA);
        cout << endl;
        cout << "Matrix B:" << endl;
        printMatrix(matrixB);
        cout << endl;
        cout << "Result Matrix C:" << endl;
        printMatrix(matrixC);
        cout << endl;
        cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
