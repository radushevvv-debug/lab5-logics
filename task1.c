#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **allocate_matrix(int n)
{
    int **matrix = malloc((size_t)n * sizeof(*matrix));

    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = malloc((size_t)n * sizeof(*matrix[i]));

        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }

            free(matrix);
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

void generate_graph(int **matrix, int n)
{
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;

        for (int j = i + 1; j < n; j++) {
            int edge = rand() % 2;

            matrix[i][j] = edge;
            matrix[j][i] = edge;
        }
    }
}

void print_matrix(int **matrix, int n)
{
    printf("\nAdjacency matrix:\n    ");

    for (int j = 0; j < n; j++) {
        printf("%3d", j + 1);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%3d ", i + 1);

        for (int j = 0; j < n; j++) {
            printf("%3d", matrix[i][j]);
        }

        printf("\n");
    }
}

size_t count_edges(int **matrix, int n)
{
    size_t count = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j] == 1) {
                count++;
            }
        }
    }

    return count;
}

int vertex_degree(const int *row, int n)
{
    int degree = 0;

    for (int j = 0; j < n; j++) {
        degree += row[j];
    }

    return degree;
}

void print_vertices_by_degree(int **matrix, int n,
                              int degree, const char *title)
{
    int found = 0;

    printf("%s:", title);

    for (int i = 0; i < n; i++) {
        if (vertex_degree(matrix[i], n) == degree) {
            printf(" %d", i + 1);
            found = 1;
        }
    }

    if (!found) {
        printf(" none");
    }

    printf("\n");
}

void free_matrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }

    free(matrix);
}

int main(void)
{
    int n;

    printf("Enter the number of vertices: ");
    fflush(stdout);

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "The number of vertices must be positive\n");
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));

    int **matrix = allocate_matrix(n);

    generate_graph(matrix, n);
    print_matrix(matrix, n);

    printf("\nGraph size (number of edges): %zu\n",
           count_edges(matrix, n));

    printf("\nVertex degrees:\n");

    for (int i = 0; i < n; i++) {
        printf("Vertex %d: %d\n", i + 1,
               vertex_degree(matrix[i], n));
    }

    print_vertices_by_degree(matrix, n, 0,
                             "Isolated vertices");

    print_vertices_by_degree(matrix, n, 1,
                             "Pendant vertices");

    print_vertices_by_degree(matrix, n, n - 1,
                             "Dominating vertices");

    free_matrix(matrix, n);

    return 0;
}