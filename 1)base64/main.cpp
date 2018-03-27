#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void makeAlphabet (unsigned char * alphabet) {
    int i = 0;
    for (i = 0; i <= 25; i++)
        alphabet[i] = 'A' + i;
    for (i = 0; i <= 25; i++)
        alphabet[26 + i] = 'a' + i;
    for (i = 0; i <= 10; i++)
        alphabet[52 + i] = '0' + i;
    alphabet[62] = '+';
    alphabet[63] = '/';
}

void encode (unsigned char input [3], unsigned char output [4], int toCode) {
    unsigned char alphabet[64];
    int i = 0;
    makeAlphabet(alphabet);
    unsigned char temp;
    output [0] = alphabet [input [0] >> 2];
    for (i = 0; i < toCode; i++) {
        temp = (input [i] << (3 - i) * 2); //6 2- 4 2- 2 2
        temp = temp >> 2;
        if (i != 2) {
            temp = temp + (input [i+1] >> ((i+2) * 2));
        }
        output [i + 1] = alphabet [temp];
    }
    for (i = 0; i < (3 - toCode); i++) {
        output [3 - i] = '=';
    }
}

int decode (unsigned char input [4], unsigned char output [3]) {
    int codedSymbols = 0;
    unsigned char alphabet[64];
    unsigned char temp;
    int i = 0;
    int z = 0;
    int breakNumber = 4;
    makeAlphabet (alphabet);
    for (i = 0; i < 4; i++)
        if (input [i] != '=') {
            for (z = 0; z < 64; z++)
                if (alphabet[z] == input[i]) {
                    input [i] = z;
                    break;
                }
        }
        else
            if (breakNumber == 4)
                breakNumber = i;
    for (i = 0; i < breakNumber - 1; i++) {
        temp = (input [i] << (2 * (i + 1)));
        temp = temp + (input [i + 1] >> (6 - 2 * (i + 1)));
        output [i] = temp;
    }
    codedSymbols = i;
    return codedSymbols;
}

int main (int argc, char *argv[]) {
    int i = 1;
    long int j;
    int codeFlag = -1;
    int ignoreFlag = 0;
    int codedSymbols;
    int fFlag = 0;
    int fNumber = -1;
    int fCounter = 0;
    unsigned char * wholeFile;
    unsigned char arr4 [4];
    unsigned char arr3 [3];
    char currentString [20];
    long int fileSize;
    if (argc > 1) {
        for (i; i < argc - 2; i++) {
            strcpy(currentString, argv[i]);
            if (currentString[0] == '-') {
                switch (currentString[1]) {
                    case 'e':
                        codeFlag = 0;
                        break;
                    case 'd':
                        codeFlag = 1;
                        break;
                    case 'i':
                        ignoreFlag = 1;

                        break;
                    case 'f':
                        fFlag = 1;
                        i++;
                        strcpy(currentString, argv[i]);
                        fNumber = atoi(currentString);
                        break;
                }
            }
        }
        int bufferCounter = 0;
        FILE *infile = fopen(argv[i], "rb");
        FILE *outfile = fopen(argv[i + 1], "wb");
        fseek(infile, 0, SEEK_END);
        fileSize = ftell(infile);
        rewind(infile);
        wholeFile = (unsigned char *) malloc(sizeof(unsigned char) * fileSize);
        fread(wholeFile, sizeof(unsigned char), fileSize, infile);
        if (codeFlag == 0) {
            for (j = 0; j < fileSize; j++) {
                arr3[j % 3] = wholeFile[j];
                if (j % 3 == 2) {
                    encode(arr3, arr4, 3);
                    for (i = 0; i < 4; i++) {
                        fputc (arr4[i], outfile);
                        fCounter++;
                        if (fCounter % fNumber == 0 && fFlag == 1)
                            fputc ('\n', outfile);
                    }
                    //fwrite(arr4, sizeof(unsigned char), 4, outfile);
                }
            }
            if (j % 3 != 0) {
                for (int k = 0; k < 3 - j % 3; k++) {
                    arr3 [2-k] = 0;
                }
                encode(arr3, arr4, j % 3);
                for (i = 0; i < 4; i++) {
                    putc (arr4[i], outfile);
                    fCounter++;
                    if (fCounter % fNumber == 0 && fFlag == 1)
                        putc ('\n', outfile);
                }
                //fwrite(arr4, sizeof(unsigned char), 4, outfile);
            }
        } else {
            if (codeFlag == 1) {
                for (j = 0; j < fileSize; j++) {
                    if (j >= fileSize)
                        break;
                    if ((wholeFile[j] <= 'z' && wholeFile[j] >='a') || (wholeFile[j] <= 'Z' && wholeFile[j] >='A') || (wholeFile[j] <= '9' && wholeFile[j] >='0') || wholeFile [j] == '+' || wholeFile [j] == '/' || wholeFile[j] == '=') {
                        arr4[bufferCounter % 4] = wholeFile[j];
                        if (bufferCounter % 4 == 3) {
                            codedSymbols = decode(arr4, arr3);
                            fwrite(arr3, sizeof(unsigned char), codedSymbols, outfile);
                        }
                        bufferCounter++;
                    }
                    else {
                        if (ignoreFlag == 0) {
                            printf("ERROR - WRONG SYMBOL 1) %c 2)", wholeFile[j]);
                            return -1;
                        }
                        else {
                            continue;
                        }
                    }
                }
            } else {
                printf("ERROR-NO FLAGS");
            }
        }
    }
    else
        printf ("ERROR-TOO FEW ARGUMENTS");
    return 0;
}