#include <stdlib.h>
#include <matrix.h>

void free_matrix(Matrix *m) {
    free(m->numbers);
    free(m);
}

void free_matrices(Matrix **matrices, int matrices_count) {
    for (int i = 0; i < matrices_count; i++) {
        free_matrix(matrices[i]);
    }
}

Matrix *alloc_matrix(int rows, int columns) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->rows = rows;
    m->columns = columns;
    m->numbers = (int*)calloc((rows * columns), sizeof(int));
    
    return m;
}

Matrix *copy_matrix(Matrix *src) {
    Matrix *dst = (Matrix*)malloc(sizeof(Matrix));
    dst->rows = src->rows;
    dst->columns = src->columns;
    dst->numbers = (int*)malloc(dst->columns * dst->rows * sizeof(int));

    for (int i = 0; i < src->rows * src->columns; i++) {
        dst->numbers[i] = src->numbers[i];
    }

    return dst;
}

Matrix *add_matrices(Matrix *m1, Matrix *m2) {
    
    for (int j = 0; j < number_of_items; j++) {
        resulted_matrix->numbers[j] += m2->numbers[j];
    }
}
Matrix *sub_matrices(Matrix *m1, Matrix *m2);
Matrix *mult_matrices(Matrix *m1, Matrix *m2);

void print_matrix(Matrix *m) {
    printf("%d %d\n", m->rows, m->columns);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            printf("%d ", m->numbers[i * m->columns + j]);
        }
        printf("\n");
    }
}