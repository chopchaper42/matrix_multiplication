#ifndef MATRIX_H
#define MATRIX_H

#ifndef MATRIX
#define MATRIX

typedef struct {
    int rows, columns;
    int *numbers;
    Matrix *next;
} Matrix;

#endif

void free_matrix(Matrix *m);
void free_matrices(Matrix **matrices, int matrices_count);
Matrix *alloc_matrix(int rows, int columns, Matrix *next);
Matrix *copy_matrix(Matrix *src);

Matrix *add_matrices(Matrix *m1, Matrix *m2);
Matrix *sub_matrices(Matrix *m1, Matrix *m2);
Matrix *mult_matrices(Matrix *m1, Matrix *m2);

void print_matrix(Matrix *m);

#endif
