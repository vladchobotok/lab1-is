#include <iostream>
#include <omp.h>

using namespace std;

const int MATRIX_SIZE = 2000;
int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int matrixC[MATRIX_SIZE][MATRIX_SIZE];

void generateMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
#pragma omp parallel for collapse(2)
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

void multiplyMatrix(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE][MATRIX_SIZE], int c[MATRIX_SIZE][MATRIX_SIZE]) {
#pragma omp parallel for collapse(2)
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            c[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    omp_set_num_threads(1);
    double start_time, end_time;

    generateMatrix(matrixA);
    generateMatrix(matrixB);

    start_time = omp_get_wtime(); // Початок вимірювання часу

    multiplyMatrix(matrixA, matrixB, matrixC);

    end_time = omp_get_wtime(); // Кінець вимірювання часу

//    cout << "Matrix A:" << endl;
//    printMatrix(matrixA);
//    cout << endl;
//    cout << "Matrix B:" << endl;
//    printMatrix(matrixB);
//    cout << endl;
//    cout << "Result Matrix C:" << endl;
//    printMatrix(matrixC);
//    cout << endl;
    cout << "Time taken: " << end_time - start_time << " seconds" << endl; // Виведення часу виконання

    return 0;
}
