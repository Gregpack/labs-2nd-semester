#include <stdio.h>
#include <stdlib.h>

struct edge {
    int length;
    int vertex1;
    int vertex2;
};


int compare (const void * a, const void * b)
{
    return (((struct edge*)a)->length - ((struct edge*)b)->length);
}

int kruskal(struct edge * allEdges, int vertices, int edges, int * vertexColour, int ** output){
    int flag = 0;
    for (int i = 0; i < edges; i++) {
        if (vertexColour[allEdges[i].vertex1 - 1] != vertexColour[allEdges[i].vertex2 - 1] ) {
            flag++;
            output[0][flag - 1] = allEdges[i].vertex1;
            output[1][flag - 1] = allEdges[i].vertex2;
            if (flag == vertices - 1)
                break;
            for (int j = 0; j < vertices; j++)
                if (vertexColour[j] == vertexColour[allEdges[i].vertex2 - 1])
                    vertexColour[j] = vertexColour[allEdges[i].vertex1 - 1];
        }
    }
    return flag;
}

int main(int argc, char **argv) {
    FILE * input;
    FILE * output;
    input = fopen(argv[1], "r");
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
    struct edge * allEdges = (struct edge*) malloc (sizeof(struct edge) * edges);
    int * vertexColour = (int*) malloc (sizeof(int) * vertices);
    for (int i = 0; i < vertices; i++)
        vertexColour[i] = i;
    for (int i = 0; i < edges; i++) {
        error_check = fscanf(input, "%d %d %d", &allEdges[i].vertex1, &allEdges[i].vertex2, &allEdges[i].length);
        if (error_check != 3) {
            fprintf(output, "bad number of lines");
            return 0;
        }
        if (allEdges[i].vertex1 < 1 || allEdges[i].vertex2 < 1 || allEdges[i].vertex1 > vertices + 1 || allEdges[i].vertex2 > vertices + 1) {
            fprintf(output, "bad vertex");
            return 0;
        }
        if (allEdges[i].length < 0) {
            fprintf(output, "bad length");
            return 0;
        }
    }
    qsort (allEdges, edges, sizeof(struct edge), compare);
    int * outputArr [2];
    for (int i = 0; i < 2; i++)
        outputArr[i] = (int*) malloc(sizeof(int) * (vertices - 1));
    if (kruskal(allEdges, vertices, edges, vertexColour, outputArr) == (vertices - 1))
        for (int i = 0; i < vertices - 1; i++)
            fprintf(output, "%d %d\n", outputArr[0][i], outputArr[1][i]);
    else
        fprintf(output, "no spanning tree");
    return 0;
}