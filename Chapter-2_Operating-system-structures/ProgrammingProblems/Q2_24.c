/*
Extract input file name and output file name.
if input file doesn't exist:
    exit with error
if output file exists:
    Request user if file needs to be over-written.
    if yes:
        delete file
    if no:
        exit process
create file
while charachter is not EOF :
    get charachter from input file
    write onto output file
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int exists(char * fileName) {
    FILE * file;
    if ((file = fopen(fileName, "r"))) {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc, char * argv[]) {
    if (argc != 3) {
        printf("ERR : Missing file name.\n");
        return 0;
    }
    char * cmd = argv[0], * inp = argv[1], * out = argv[2];
    printf("Copying contents from input file : %s to output file : %s\n", inp, out);

    if (!exists(inp)) {
        printf("ERR: Input file does not exist.\n");
        return 1;
    }
    if (exists(out)) {
        printf("Output File already exists. Do you want to overwrite it? (Y/n) ");
        char overWrite;
        scanf("%c", &overWrite);
        if (tolower(overWrite) != 'y' ) {
            printf("Exiting copy.\n");
            return 0;
        }
        remove(out);
    }
    
    FILE *inpFile = fopen(inp, "r"), *outFile = fopen(out, "w");
    if (inpFile == NULL && outFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    int ch;

    while ((ch = fgetc(inpFile)) != EOF) {
        fputc((char)ch, outFile);
    }

    fclose(inpFile);
    fclose(outFile);
    printf("Copy successfull\n");
    return 0;
}