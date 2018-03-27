#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
struct treeNode {
    struct treeNode * left;
    struct treeNode * right;
    unsigned char element;
};

struct queueP {
    int isChar;
    struct treeNode *treeElem;
    unsigned char element;
    int key;
    struct queueP *next;
};

struct queueP * pop (struct queueP * queue) {
    struct queueP * temp;
    if (queue == queue->next) {
        temp = NULL;
        return temp;
    }
    if (queue != NULL) {
        temp = queue;
        while (temp->next != queue) {
            temp = temp->next;
        }
        temp->next=temp->next->next;
        return temp;
    }
}

struct queueP * insert (struct queueP * tail, struct treeNode * treeEl, unsigned char elem, int key, int isChar) {
    struct queueP * queue = (struct queueP *) malloc(sizeof(struct queueP));
    if (tail != NULL) {
        queue->next = tail->next;
        tail->next = queue;
    }
    else
        queue->next = queue;
    queue->key = key;
    if (isChar == 1) {
        queue->element = elem;
        queue->isChar = 1;
    }
    else {
        queue->treeElem = treeEl;
        queue->isChar = 0;
    }
    return queue;
}

struct queueP * extractMinimum (struct queueP * queue, struct queueP * answer) {
    if (queue == NULL)
        return NULL;
    struct queueP * counter = queue;
    struct queueP * minElem;
    int min = INT_MAX;
    do {
        if (counter->key < min) {
            min = counter->key;
            minElem = counter;
        }
        counter = counter->next;
    } while (counter != queue);
    answer->treeElem = minElem->treeElem;
    answer->key = minElem->key;
    answer->element = minElem->element;
    answer->isChar = minElem->isChar;
    queue = pop (minElem);
    return queue;
}

struct treeNode * link (struct queueP * elem1, struct queueP * elem2) {
    struct treeNode * node = (struct treeNode *) malloc(sizeof(struct treeNode));
    node->left = NULL;
    node->right = NULL;
    //node->isLeaf = 0;
    switch (elem1->isChar) {
        case 1: {
            struct treeNode * node1 = (struct treeNode *) malloc(sizeof(struct treeNode));
            node1->element = elem1->element;
            node->left = node1;
            node1->left = NULL;
            node1->right = NULL;
            break;
        }
        case 0: {
            node->left = elem1->treeElem;
            break;
        }
    }
    switch (elem2->isChar) {
        case 1: {
            struct treeNode * node2 = (struct treeNode *) malloc(sizeof(struct treeNode));
            node2->element = elem2->element;
            node->right = node2;
            node2->left = NULL;
            node2->right = NULL;
            break;
        }
        case 0: {
            node->right = elem2->treeElem;
            break;
        }
    }
    return node;
}

unsigned char * actualEncode (struct treeNode * elem, unsigned char input, unsigned char * currentSymbol, int * length) {
    unsigned char * result = (unsigned char*) malloc (sizeof(unsigned char) * 256);
    int * length1;
    length1 = length;
    if (elem->left == NULL && elem->right == NULL && elem->element != input) {
        result [0] = 0;
        *length = *length - 1;
        return result;
    }
    if (elem->element == input && elem->left == NULL && elem->right == NULL) {
        return currentSymbol;
    }
    else {
        currentSymbol [*length] = '0';
        *length = *length + 1;
        result = actualEncode(elem->left, input, currentSymbol, length1);
        if (result [0] != 0)
            return result;
        currentSymbol [*length] = '1';
        *length = *length + 1;
        result = actualEncode(elem->right, input, currentSymbol, length1);
        if (result [0] != 0)
            return result;
    }
    result [0] = 0;
    *length = *length - 1;
    return result;
}

int tableCreation (unsigned char *table [256], struct treeNode * treeHuff, int frequency [256]) {
    unsigned char * strBuffer = (unsigned char *) malloc (sizeof(unsigned char) * 256);
    int maxlen = 0;
    int length = 0;
    for (int i = 0; i < 256; i++) {
        if (frequency [i] == 0)
            table [i] = NULL;
        else {
            actualEncode(treeHuff, i, strBuffer, &length);
            table[i] = (unsigned char*) malloc (sizeof(unsigned char) * (length+1));
            strncpy ((char*)table[i], (const char *)strBuffer, sizeof(unsigned char) * length);
            table[i][length] = 0;
            if (length > maxlen)
                maxlen = length;
            length = 0;
        }
    }
    free (strBuffer);
    return maxlen;
}

void systemStringCreation (struct treeNode * elem, unsigned char * outputstr, int * length){
    if (elem->left == NULL || elem->right == NULL) {
        outputstr [*length] = '1';
        *length = *length + 1;
        outputstr [*length] = elem->element;
        *length = *length + 1;
    }
    else {
        outputstr [*length] = '0';
        *length = *length + 1;
        systemStringCreation(elem->left, outputstr, length);
        outputstr [*length] = '0';
        *length = *length + 1;
        systemStringCreation(elem->right, outputstr, length);
    }
}

void treeRebuild (struct treeNode * tree, unsigned char * systemString, int length, int * currentLength) {
    if (*currentLength == length)
        return;
    while (*currentLength != length) {
        if (systemString [* currentLength] == '0') {
            struct treeNode* node = (struct treeNode *) malloc(sizeof(struct treeNode));
            if (tree->left == NULL)
                tree->left = node;
            else {
                if (tree->right == NULL)
                    tree->right = node;
                else {
                    free (node);
                    return;
                }
            }
            node->left = NULL;
            node->right = NULL;
            *currentLength = *currentLength + 1;
            treeRebuild (node, systemString, length, currentLength);
        }
        else {
            if (systemString [* currentLength] == '1') {
                *currentLength = *currentLength + 1;
                tree->left = NULL;
                tree->right = NULL;
                tree->element =  systemString [* currentLength];
                *currentLength = *currentLength + 1;
                return;
            }
        }
    }
}

void decode (FILE * infile, FILE * outfile){
    unsigned char readBuffer [1024];
    unsigned char output [1024];
    double loading = 0;
    int symbolsRead = 0;
    long long int outputSize = 0;
    int treeSize = 0; //размер системной строки
    unsigned char currentSymbol;
    int i = 0;
    int j = 0;
    int length = 0;
    while ((currentSymbol = fgetc(infile)) != ' ') {
        outputSize = outputSize * 10 + (currentSymbol - '0');
    }
    while ((currentSymbol = fgetc(infile)) != ' ') {
        treeSize = treeSize * 10 + (currentSymbol - '0');
    }
    struct treeNode * tree = (struct treeNode *) malloc(sizeof(struct treeNode));
    tree->left = NULL;
    tree->right = NULL;
    unsigned char * systemString = (unsigned char *) malloc(sizeof(unsigned char) * treeSize);
    fread (systemString, sizeof(unsigned char), treeSize, infile);
    treeRebuild(tree, systemString, treeSize, &length);
    struct treeNode * temp = tree;
    int decodedSymbols = 0;
    int total = 0;
    while (((symbolsRead = fread(readBuffer, sizeof(unsigned char), 1024, infile)) != 0) || total != outputSize)  {
        //printf ("loading : %f %% \n", (loading/outputSize)*100);
        //loading = loading + 1024;
        if (symbolsRead == 0) {
            for (i = total; i < outputSize; i++) {
                fputc (temp->element, outfile);
                total++;
            }
        }
        else {
            for (i = 0; i < symbolsRead; i++) {
                currentSymbol = readBuffer [i];
                for (j = 7; j >= 0; j--) {
                    if (temp->left == NULL && temp->right == NULL) {
                        output [decodedSymbols] = temp->element;
                        decodedSymbols++;
                        if (decodedSymbols == 1024) {
                            fwrite(output, sizeof(unsigned char), decodedSymbols, outfile);
                            decodedSymbols = 0;
                        }
                        total++;
                        temp = tree;
                        if (total == outputSize)
                            break;
                    }
                    if ((currentSymbol / (1 << j)) == 1) {
                            temp = temp->right;
                    }
                    else {
                            temp = temp->left;
                    }
                    currentSymbol = currentSymbol - (currentSymbol / (1 << j)) * (1 << j);
                }
            }
        }
    }
    fwrite(output, sizeof(unsigned char), decodedSymbols, outfile);
}

void encode (FILE * infile, FILE * outfile) {
    int frequency [256];
    double loading = 0;
    unsigned char readBuffer [1024];
    int symbolsRead = 0;
    long long int fileSize = 0;
    unsigned char output [1024];
    long long int codedSymbols = 0;
    unsigned char *table [256];
    struct queueP *queue = NULL;
    struct queueP *elem1 = (struct queueP*) malloc(sizeof(struct queueP));
    struct queueP *elem2 = (struct queueP*) malloc(sizeof(struct queueP));
    elem1->treeElem = NULL;
    elem2->treeElem = NULL;
    int buffCounter = 0;
    int result = 0;
    int j;
    int length = 0;

    for (int i = 0; i < 256; i++)
        frequency [i] = 0;

    while ((symbolsRead = fread(readBuffer, sizeof(unsigned char), 1024, infile)) != 0) {
        for (int i = 0; i < symbolsRead; i++) {
            frequency[readBuffer[i]]++;
        }
        fileSize = fileSize + symbolsRead;
    }
    rewind (infile);
    for (int i = 0; i < 256; i++) {
        if (frequency[i] != 0) {
            queue = insert(queue, NULL, i, frequency[i], 1);
        }
    }

    while ((queue = extractMinimum(queue, elem1)) != NULL && (queue = extractMinimum(queue, elem2)) != NULL) {
        insert (queue, link (elem1, elem2), 0, elem1->key + elem2->key, 0);
    }
    if (elem2->treeElem != NULL)
        queue = insert (queue, link (elem1, elem2), 0, elem1->key + elem2->key, 0);
    else {
        queue = (struct queueP*) malloc (sizeof(struct queueP));
        queue->treeElem = (struct treeNode*) malloc (sizeof (struct treeNode));
        queue->treeElem->element = elem1->element;
        queue->treeElem->left = NULL;
        queue->treeElem->right = NULL;
    }
    unsigned char * systemString = (unsigned char*)malloc(sizeof(unsigned char) * 10000);
    int buffLen = tableCreation(table, queue->treeElem, frequency);
    systemStringCreation(queue->treeElem, systemString, &length);
    realloc(systemString, sizeof(unsigned char) * length);
    fprintf(outfile, "%lld %d ", fileSize, length);
    fwrite(systemString, sizeof(unsigned char), length, outfile);

    /*for (int i = 0; i < 256; i++)
        if (table[i] != NULL) {
            printf ("%d: ", i);
            puts ((const char*)table[i]);
        }
    */

    if (buffLen * 2 < 8)
        buffLen = 8;
    else
        buffLen = buffLen * 2;
    unsigned char * buffer = (unsigned char*) malloc(sizeof(unsigned char) * buffLen);
    buffer [0] = 0;
    double total = fileSize;
    while ((symbolsRead = fread(readBuffer, sizeof(unsigned char), 1024, infile)) != 0) {
       // printf ("loading : %f %% \n", (loading/total)*100);
        //loading = loading + 1024;
        for (int i = 0; i < symbolsRead; i++) {
            strcat((char *) buffer, (const char *) table[readBuffer[i]]);
            buffCounter = buffCounter + strlen((const char *) table[readBuffer[i]]);
            while (buffCounter >= 8) {
                for (j = 7; j >= 0; j--) {
                    result = result + (1 << j) * (buffer[7 - j] - '0');
                }
                output[codedSymbols] = result;
                codedSymbols++;
                result = 0;
                for (j = 0; j <= buffCounter - 8; j++)
                    buffer[j] = buffer[j + 8];
                for (j; j < buffLen; j++)
                    buffer[j] = 0;
                buffCounter = buffCounter - 8;
            }
        }
        fwrite (output, sizeof(unsigned char), codedSymbols, outfile);
        codedSymbols = 0;
    }

    if (buffCounter != 0) {
        for (buffCounter; buffCounter < 8; buffCounter++) {
            buffer[buffCounter] = '0';
        }
        for (j = 7; j >= 0; j--) {
            result = result + (1 << j) * (buffer[7 - j] - '0');
        }
        fputc (result, outfile);
    }
    free (elem1);
    free (elem2);
    free (queue);
}

int main (int argc, char *argv[]) {
    char flag[1];

    strcpy(flag, argv[1]);
    FILE * input = fopen (argv[2], "rb");
    FILE * output = fopen (argv[3], "wb");

    if (flag [0] == 'e')
        encode (input, output);
    else
    if (flag [0] == 'd')
        decode(input, output);
    printf ("it took %d seconds to do it \n", clock() / CLOCKS_PER_SEC);
    fclose (input);
    fclose (output);

    return 0;
}