#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int extractMinimum (int * distances, int vertices, int * isInQueue) {
    int minimum = INT_MAX;
    int minimumNumber = -1;
    for (int i = 0; i < vertices; i++) {
        if (distances [i] < minimum && isInQueue[i] == 1) {
            minimum = distances[i];
            minimumNumber = i;
        }
    }
    if (minimum != INT_MAX) {
        isInQueue[minimumNumber] = 0;
        return minimumNumber;
    }
    else return -1;
}

void out (FILE * output, int * born, int vertices) {
    if (vertices == 1)
        fprintf(output, "1");
    for (int i = 0; i < vertices; i++)
        if (born[i] == -1)
            continue;
        else
            fprintf(output, "%d %d\n", born[i] + 1, i + 1);
}

int prim (int** connections, int * distances, int vertices, int * born){
    int currentVertex;
    int elements = vertices;
    int * isInQueue = (int*) malloc (sizeof(int) * vertices);
    for (int i = 0; i < vertices; i++)
        isInQueue[i] = 1;
    do{
        currentVertex = extractMinimum(distances, vertices, isInQueue);
        if (currentVertex == -1)
            return -1;
        for (int i = 0; i < vertices; i++){
            if (connections[currentVertex][i] != -1) {
                if (isInQueue[i] && (connections [currentVertex][i] < distances[i])) {
                    distances [i] = connections[currentVertex][i];
                    born [i] = currentVertex;
                }
            }
        }
        --elements;
    } while (elements != 0);
    return 0;
}

int main(int argc, char** argv) {
    FILE * input;
    FILE * output;
    int vert1, vert2, dist;
    input = fopen (argv[1], "r");
    output = fopen (argv[2], "w");
    int vertices = 0, edges = 0, error_check;
    fscanf (input, "%d %d", &vertices, &edges);
    if (vertices > 5000 || vertices < 0) {
        fprintf(output, "bad number of vertices");
        return 0;
    }
    if (edges > vertices * (vertices - 1) / 2 || edges < 0) {
        fprintf(output, "bad number of edges");
        return 0;
    }
    int * distances = (int *) malloc (sizeof (int) * vertices);
    int * born = (int *) malloc (sizeof (int) * vertices);
    int ** connections = (int **) malloc (sizeof(int*) * vertices);
    for (int i = 0; i < vertices; i++)
        connections[i] = (int *) malloc (sizeof(int) * vertices);
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++)
            connections[i][j] = -1;
    }
    for (int i = 0; i < vertices; i++){
        distances[i] = INT_MAX;
        born[i] = -1;
    }
    distances[0] = 0;

    for (int i = 0; i < edges; i++) {
        if (fscanf (input, "%d %d %d", &vert1, &vert2, &dist) != 3) {
            fprintf(output, "wrong parameters");
            return 0;
        }
        else {
        --vert1;
        --vert2;
        connections[vert1][vert2] = dist;
        connections[vert2][vert1] = dist;
        }
    }
    if (prim (connections, distances, vertices, born) == -1)
        fprintf(output, "no spanning tree");
    else
        out(output, born, vertices);
    return 0;
}