#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows, columns;
    int *numbers;
    Matrix *next;
} Matrix;

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

void copy_matrix(Matrix *src, Matrix *dst) {
    dst->rows = src->rows;
    dst->columns = src->columns;
    dst->numbers = (int*)malloc(dst->columns * dst->rows * sizeof(int));

    for (int i = 0; i < src->rows * src->columns; i++) {
        dst->numbers[i] = src->numbers[i];
    }
}

void print_matrix(Matrix *m) {
    printf("%d %d\n", m->rows, m->columns);
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
                    printf("read: %d\n", m->numbers[matrix_index]);
                    if (read_result != 1) {
                        fprintf(stderr, "Error: Chybny vstup!\n");
                        return 100;
                    }
                }
            }

            // check if need to increase memory
            if ((float)matrices_count / matrices_allocated > 0.8) {
                //printf("Matrices count: %d\nMemory allocated for: %d\nIncreasing.\n", matrices_count, matrices_allocated);
                matrices_allocated += 5;
                matrices = realloc(matrices, matrices_allocated * sizeof(Matrix));
            }

        } else {
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

    printf("-----------\n");

    for (int i = 0; i < operators_count; i++) {
        if (operators[i] != '*') continue;

        Matrix *m1 = matrices[i];
        Matrix *m2 = matrices[i + 1];
        Matrix *resulted_matrix = (Matrix*)malloc(sizeof(Matrix));
        copy_matrix(m1, resulted_matrix);

        resulted_matrix->columns = m2->columns;
        resulted_matrix->numbers = (int*)realloc(resulted_matrix->numbers, resulted_matrix->rows * resulted_matrix->columns * sizeof(int));            

        for (int j = 0; j < resulted_matrix->rows; j++) {
            for (int k = 0; k < resulted_matrix->columns; k++) {
                int result = 0;
                for (int l = 0; l < m2->rows; l++) {
                    result += m1->numbers[j * m1->columns + l] * m2->numbers[l * m2->columns + k];
                }
                resulted_matrix->numbers[j * resulted_matrix->columns + k] = result;
            }
        }

        matrices[i] = NULL;
        free(matrices[i + 1]);
        matrices[i + 1] = resulted_matrix;
    }

    Matrix *resulted_matrix = (Matrix*)malloc(sizeof(Matrix));
    copy_matrix(matrices[0], resulted_matrix);

    for (int index = 0, op_index = 0; index < matrices_count - 1; index++, op_index++) {
        while (matrices[index] == NULL)
            index++;

        Matrix *m2 = matrices[index + 1];
        int number_of_items = m2->columns * m2->rows;

        if (operators[op_index] == '+') {
            if (resulted_matrix->columns != m2->columns || resulted_matrix->rows != m2->rows) {
                fprintf(stderr, "Error: Chybny vstup +!\n");
                fprintf(stderr, "%d != %d || %d != %d\n", resulted_matrix->columns, m2->columns, resulted_matrix->rows, m2->rows);
                return 100;
            }
            for (int j = 0; j < number_of_items; j++) {
                resulted_matrix->numbers[j] += m2->numbers[j];
            }
        }
        if (operators[op_index] == '-') {
            if (resulted_matrix->columns != m2->columns || resulted_matrix->rows != m2->rows) {
                fprintf(stderr, "Error: Chybny vstup -!\n");
                fprintf(stderr, "%d != %d || %d != %d\n", resulted_matrix->columns, m2->columns, resulted_matrix->rows, m2->rows);
                return 100;
            }
            for (int j = 0; j < number_of_items; j++) {
                resulted_matrix->numbers[j] -= m2->numbers[j];
            }
        }
        if (operators[op_index] == '*') {
            if (resulted_matrix->columns != m2->rows) {
                fprintf(stderr, "Error: Chybny vstup!\n");
                fprintf(stderr, "%d != %d\n", resulted_matrix->columns, m2->rows);
                return 100;
            }
            // make a copy, to take original values from and put the result in resulted_matrix
            Matrix *copy = (Matrix*)malloc(sizeof(Matrix));
            copy_matrix(resulted_matrix, copy);

            // reformate the resulted matrix
            resulted_matrix->columns = m2->columns;
            resulted_matrix->numbers = (int*)realloc(resulted_matrix->numbers, resulted_matrix->rows * resulted_matrix->columns * sizeof(int));            
            
            // iterate over each element of the resulting matrix
            for (int j = 0; j < resulted_matrix->rows; j++) {
                for (int k = 0; k < resulted_matrix->columns; k++) {
                    int result = 0;
                    for (int l = 0; l < m2->rows; l++) {
                        result += copy->numbers[j * copy->columns + l] * m2->numbers[l * m2->columns + k];
                    }
                    resulted_matrix->numbers[j * resulted_matrix->columns + k] = result;
                }
            }

            free_matrix(copy);
        }
    }

    printf("\nResult:\n");
    print_matrix(resulted_matrix);

    free_matrices(matrices, matrices_count);
    free(matrices);
    free(operators);
    free_matrix(resulted_matrix);
    return 0;
}
