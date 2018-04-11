
#include <stdio.h>
#include <stdlib.h>

int topsort (int vertices, int **links, int * output) {
    int *check = (int*) malloc (sizeof(int) * vertices);
    for (int i = 0; i < vertices; i++)
        check[i] = 0;
    int i = 0;
    int j = 0;
    int checked = 0;
    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            if (links[j][i] != 0)
                break;
        }
        if (j == vertices && check[i] != 1) {
            check[i] = 1;
            output[checked] = i;
            checked++;
            for (int z = 0; z < vertices; z++)
                links [i][z] = 0;
            i = -1;
        }
        if (checked == vertices)
            break;
    }
    if (i == vertices && checked != vertices)
        return -1;
    else
        return 1;

}

int main(int argc, char **argv) {
    FILE *input;
    FILE *output;
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    int vertices = 0, edges = 0;
    int temp1, temp2, error = 2;

    error = fscanf(input, "%d %d", &vertices, &edges);
    if (error < 2) {
        fprintf(output, "bad number of lines");
        return 0;
    }
    int **links = (int **) malloc(sizeof(int *) * vertices);
    for (int i = 0; i < vertices; i++)
        links[i] = (int *) malloc(sizeof(int) * vertices);

    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            links[i][j] = 0;

    for (int i = 0; i < edges; i++) {
        error = fscanf(input, "%d %d", &temp1, &temp2);
        if (error < 2) {
            fprintf(output, "bad number of lines");
            return 0;
        }
        if (temp1 < 0 || temp2 < 0 || temp1 > vertices || temp2 > vertices) {
            fprintf(output, "bad vertex");
            return 0;
        }
        links[temp1 - 1][temp2 - 1] = 1;
    }

    if (vertices > 1000 || vertices < 0) {
        fprintf(output, "bad number of vertices");
        return 0;
    }

    if (edges < 0 || edges > vertices * (vertices - 1) / 2) {
        fprintf(output, "bad number of edges");
        return 0;
    }
    int * outputstr = (int*) malloc (sizeof(int) * vertices);
    for (int i = 0; i < vertices; i++)
        outputstr[i] = 0;
    if (topsort(vertices, links, outputstr) == -1)
        fprintf(output, "can`t be sorted");
    else
        for (int i = 0; i < vertices; i++)
            fprintf (output, "%d ", outputstr[i] + 1);
    return 0;
}