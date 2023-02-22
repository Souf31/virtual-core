#include <stdio.h>
#include <stdlib.h>

void decode(unsigned char* bytes, int bytesLength){
    int bits[31];
    for (int i=0; i<bytesLength; i=i+4){
    unsigned char a=bytes[i], b=bytes[i+1], c=bytes[i+2], d=bytes[i+3];
    unsigned long combined = (a << 24) | (b << 16) | (c << 8) | d;
    for (int i=31; i>=0; i--){
    bits[i]=(combined & (1 << i)) ? 1:0;
    printf("%d",bits[i]);
    printf("|");}
    printf("\n");
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
    // for (i = 0; i < size; i++) {
    //     printf("Data[%d] = ", i);
    //     printf("%d", data[i]);
    //     printf("\n");
    // }

    decode(data, size);

    // Clean up
    fclose(fp);
    free(data);

    return 0;
}