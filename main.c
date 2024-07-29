#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readFile(const char* filename, long* length) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)malloc(*length + 1);
    if (!data) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fread(data, 1, *length, file);
    fclose(file);
    data[*length] = '\0';
    
    return data;
}

char* compressRLE(const char* data, long length, long* compressedLength) {
    char *compressedData = (char*)malloc(2 * length); // Worst case: no compression
    if (!compressedData) {
        perror("Memory allocation failed");
        return NULL;
    }

    long i = 0, j = 0;
    while (i < length) {
        char currentChar = data[i];
        long runLength = 1;

        while (i + runLength < length && data[i + runLength] == currentChar) {
            runLength++;
        }

        compressedData[j++] = currentChar;
        compressedData[j++] = runLength;

        i += runLength;
    }

    *compressedLength = j;
    return compressedData;
}

void writeFile(const char* filename, const char* data, long length) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Could not open file");
        return;
    }

    fwrite(data, 1, length, file);
    fclose(file);
}
