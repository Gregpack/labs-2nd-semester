#include <stdio.h>
#include <stdlib.h>
#define WHITE 1
#define GREY 0
#define BLACK -1
//stack
struct list {
    int value;
    struct list *next;
};

typedef struct stack {
    struct list *top;
} stack;

stack *create () {
    stack * stk;
    stk = (stack *) malloc (sizeof(stack));
    stk->top = NULL;
    return stk;
}

int pop (stack *stk) {
    int elem;
    struct list *lst;
    lst = stk->top;
    elem = lst->value;
    stk->top = lst->next;
    free (lst);
    return elem;
}

void push (stack *stk, int elem) {
    struct list *lst;
    lst=(struct list*) malloc (sizeof (struct list));
    lst->value = elem;
    lst->next = stk->top;
    stk->top = lst;
}

int ifEmpty (stack *stk) {
    return (stk->top == NULL);
}
//stack

int dfsort (int vertex, int **links, int vertices, stack * stk, int *iswhite) {
    iswhite[vertex] = GREY;
    for (int j = 0; j < vertices; j++) {
        if (links [vertex][j] == 1){
            if (iswhite[j] == GREY)
                return -1;   //impossible to sort
            else
            if (iswhite[j] == WHITE)
                if (dfsort(j, links, vertices, stk, iswhite) == -1)
                    return -1;
        }
    }
    push (stk, vertex);
    iswhite[vertex] = BLACK;
    return 0;
}

void topsort(int vertices, int ** links, FILE * output) {
    stack *stk;
    stk = create();
    int *iswhite = (int*) malloc (sizeof(int) * vertices);
    for (int i = 0; i < vertices; i++)
        iswhite[i] = WHITE;
    for (int i = 0; i < vertices; i++) {
        if (iswhite[i] == WHITE)
            if (dfsort(i, links, vertices, stk, iswhite) == -1) {
                fprintf(output, "Impossible to sort");
                return;
            }
    }
    while (!ifEmpty(stk))
        fprintf(output, "%d ", pop(stk) + 1);
}

int main(int argc, char **argv) {
    FILE * input;
    FILE * output;
    input = fopen(argv[1], "r");
    output = fopen (argv[2], "w");
    int vertices = 0, edges = 0;
    int temp1, temp2, error = 2;

    error = fscanf(input, "%d %d", &vertices, &edges);
    if (error < 2) {
        fprintf(output, "bad number of lines");
        return 0;
    }
    int ** links = (int**) malloc (sizeof(int*) * vertices);
    for (int i = 0; i < vertices; i++)
        links[i] = (int*) malloc (sizeof(int) * vertices);

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
        links[temp1-1][temp2-1] = 1;
    }

    if (vertices > 1000 || vertices < 0) {
        fprintf(output, "bad number of vertices");
        return 0;
    }

    if (edges < 0 || edges > vertices * (vertices - 1 ) / 2) {
        fprintf(output, "bad number of edges");
        return 0;
    }

    topsort(vertices, links, output);

    return 0;
}