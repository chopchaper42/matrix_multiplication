#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows, columns;
    int *numbers;
} Matrix;

void free_matrix(Matrix *m) {
    free(m->numbers);
    free(m);
}

void free_matrices(Matrix **matrices, int length) {
    for (int i = 0; i < length; i++) {
        free_matrix(matrices[i]);
    }
}

Matrix *alloc_matrix(int rows, int columns) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->rows = rows;
    m->columns = columns;
    m->numbers = (int*)malloc((rows * columns) * sizeof(int));

    return m;
}

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            printf("%d ", m->numbers[i * m->columns + j]);
        }
        printf("\n");
    }
}


/* The main program */
int main(int argc, char *argv[])
{
    Matrix resulted_matrix;

    // array of operators (chars). Default size = 5;
    char *operators = malloc(5);
    int operators_count = 0;
    int operators_allocated = 5;

    // array of pointers to structs. Default size = 3;
    Matrix **matrices = (Matrix**)malloc(3 * sizeof(Matrix));
    int matrices_count = 0;
    int matrices_allocated = 3;

    int read_result;
    int rows, columns;

    while (1) {
        if (matrices_count == operators_count) {
            //printf("Entering a matrix\n");
            Matrix *m;
            
            //printf("Enter number of rows and number of columns:\n");
            // read number of rows and number of columns
            read_result = scanf("%d %d", &rows, &columns);

            // check if the data is OK
            if (read_result != 2) {
                return 100;
            }

            // allocate memory for the matrix
            m = alloc_matrix(rows, columns);

            if (m == NULL) {
                fprintf(stderr, "Not enough memory for a matrix!");
                return 101;
            }

            // matrices_count always equals to the next free index
            matrices[matrices_count] = m;
            matrices_count++;


            // read the matrix
            for (int row = 0; row < rows; row++) {
                for (int column = 0; column < columns; column++) {
                    int matrix_index = row * m->columns + column;
                    read_result = scanf("%d", &m->numbers[matrix_index]);
                }
            }

            /* printf("Read:\n");
             // read the matrix
            for (int row = 0; row < rows; row++) {
                for (int column = 0; column < columns; column++) {
                    int matrix_index = row * m->columns + column;
                    printf("[%d] %d ", matrix_index, m->numbers[matrix_index]);
                }
            }
            printf("\nEnd.\n"); */

            // check if need to increase memory
            if ((float)matrices_count / matrices_allocated > 0.8) {
                //printf("Matrices count: %d\nMemory allocated for: %d\nIncreasing.\n", matrices_count, matrices_allocated);
                matrices_allocated += 5;
                matrices = realloc(matrices, matrices_allocated * sizeof(Matrix));
            }
        } else {
            //printf("Entering an operator\n");
            // read an operator
            read_result = scanf("\n%c", (operators + operators_count));
            
            if (read_result == EOF)
                break;

            //printf("Operator: %c\n", operators[operators_count]);
            operators_count++;

            if (operators_count == operators_allocated) {
                operators_allocated += 5;
                operators = (char*)realloc(operators, operators_allocated);
            }
        }
    }

    Matrix resulted_matrix;
    resulted_matrix.columns = matrices[0]->columns;
    resulted_matrix.rows = matrices[0]->rows;
    resulted_matrix.numbers = (int*)malloc(resulted_matrix.columns * resulted_matrix.rows * sizeof(int));

    // +,- size of the result matrix stays the same
    // * size equals <m1.columns, m2.rows>

    // + only for now
    for (int index = 0; index < matrices_count - 1; index++) {
        Matrix *m1 = matrices[index];
        Matrix *m2 = matrices[index + 1];

        if (operators[index] == '+') {
            int number_of_items = m2->columns * m2->rows;
            for (int j = 0; j < number_of_items; j++) {
                m2->numbers[j] = m1->numbers[j] + m2->numbers[j];
            }
            resulted_matrix = m2;
        }
        if (operators[index] == '-') {
            int number_of_items = m2->columns * m2->rows;
            for (int j = 0; j < number_of_items; j++) {
                m2->numbers[j] = m1->numbers[j] - m2->numbers[j];
            }
            resulted_matrix = m2;
        }
        if (operators[index] == '*') {
            int result = 0;
            for (int j = 0; j < m1->rows; j++) {
                for (int k = 0; k < m1->columns; k++) {
                    printf("element [%d][%d]:\n", j, k);
                    for (int l = 0; l < m1->columns; l++) {
                        printf("m1: %d\n", m1->numbers[j * m1->columns + l]);
                        printf("m2: %d\n", m2->numbers[l * m2->columns + k]);
                        result += m1->numbers[j * m1->columns + l] * m2->numbers[l * m2->columns + k];
                    }

                }
            }
        }
    }

    print_matrix(resulted_matrix);

    printf("freeing matrices.\n");
    free_matrices(matrices, matrices_count);
    printf("free matrices.\n");
    free(matrices);
    printf("freeing operators.\n");
    free(operators);
    return 0;
}
