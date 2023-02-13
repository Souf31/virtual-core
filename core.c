#include <stdio.h>
#include <stdlib.h>

void print_binary(unsigned char n) {
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (n & (1 << i)) ? 1 : 0);
    }
}

int main()
{
    FILE *fp;
    unsigned char *data;
    int size;
    int i;

    fp = fopen("binarycode.bin", "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return(-1);
    }

    // Determine the size of the file
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    // Allocate memory for the data
    data = (unsigned char*)malloc(size);
    if (data == NULL) {
        perror("Error allocating memory");
        return(-1);
    }

    // Read the data from the file
    fread(data, 1, size, fp);

    // Use the data as needed
    for (i = 0; i < size; i++) {
        printf("Data[%d] = ", i);
        print_binary(data[i]);
        printf("\n");
    }

    // Clean up
    fclose(fp);
    free(data);

    return 0;
}