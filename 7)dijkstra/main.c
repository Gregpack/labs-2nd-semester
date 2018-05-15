#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_min (int * length, int * mark,  int vertices) {
    int min, minMark;
    for (int i = 0; i < vertices; i++) {
        if (mark[i] != 1) {
            min = length [i];
            minMark = i;
            break;
        }
    }
    for (int i = 0; i < vertices; i++) {
        if (length [i] < min && mark[i] != 1) {
            min = length [i];
            minMark = i;
        }
    }
    return minMark;
}

int * dijkstra (int ** edges, int vertices, int start, int * path, int * born) {
    int * length = (int *) malloc (sizeof (int) * vertices);
    int * mark = (int *) malloc (sizeof (int) * vertices);
    for (int i = 0; i < vertices; i++)
        born[i] = -1;
    int current_vertex;
    int passed = 0;
    for (int i = 0; i < vertices; i++)
        length[i] = INT_MAX;
    for (int i = 0; i < vertices; i++)
        mark[i] = 0;

    length [start] = 0;

    while (passed != vertices) {
        current_vertex = find_min (length, mark, vertices);
        for (int i = 0; i < vertices; i++) {
            if (edges[current_vertex][i] != 0 && mark[i] != 1) {
                if (length[current_vertex] > INT_MAX - edges[current_vertex][i]){
                    if (path[i] != 1) {
                        path [i] = 2;
                        born [i] = current_vertex;
                    }
                }
                else {
                    if ((edges[current_vertex][i] + length[current_vertex]) <= length [i]) {
                        if ((edges[current_vertex][i] + length[current_vertex]) == length[i]) {
                            path[i] = 1;
                            born[i] = current_vertex;
                        }
                        else {
                            path [i] = 1;
                            length [i] = edges[current_vertex][i] + length[current_vertex];
                            born[i] = current_vertex;
                        }
                    }
                }
            }
        }
        mark[current_vertex] = 1;
        passed++;
    }
    return length;
}

int main(int argc, char ** argv) {
    FILE *input;
    FILE *output;
    int vertices;
    int start, end;
    int edgeAmount;
    int st1, end1, lng;

    input = fopen (argv[1], "r");
    output = fopen (argv[2], "w");
    fscanf (input, "%d %d %d %d", &vertices, &start, &end, &edgeAmount);
    int ** edges = (int**) malloc (sizeof(int*) * edgeAmount);
    for (int i = 0; i < vertices; i++)
        edges[i] = (int *) malloc(sizeof(int) * edgeAmount);
    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            edges[i][j] = 0;
    for (int i = 0; i < edgeAmount; i++) {
        fscanf (input, "%d %d %d", &st1, &end1, &lng);
        edges [st1 - 1][end1 - 1] = lng;
        edges [end1 - 1][st1 - 1] = lng;
    }
    int * path = (int * ) malloc (sizeof (int) * vertices);
    for (int i = 0; i < vertices; i++)
        path[i] = 0;
    path[start - 1] = 1;
    int * born = (int *) malloc (sizeof (int) * vertices);
    int * answer = dijkstra(edges, vertices, start - 1, path, born);

    for (int i = 0; i < vertices; i++)
        if (path[i] == 0)
            fprintf(output, "inf ");
        else {
            if (path[i] == 1) {
                fprintf(output, "%d ", answer[i]);
            }
            else
                fprintf(output, "INT_MAX+ ");
        }
    if (path [end - 1] == 1) {
        int i = end - 1;
        fprintf (output, "\n");
        do {
            fprintf(output, "%d ", i + 1);
            i = born[i];
        } while (i != -1);
    }
    else
        if (path [end - 1] == 0)
            fprintf(output, "\nno path ");
        else
            fprintf(output, "\noverflow");
    return 0;
}