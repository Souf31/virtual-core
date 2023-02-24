#include <stdio.h>
#include <stdlib.h>

void execute(int opcode, int iv, int dest, int ope1, int ope2, int ivflag, unsigned long* registers){
    
}



void decode(unsigned char* bytes, int bytesLength, unsigned long* registers){
    int bits[32];
    for (int i=0; i<bytesLength; i=i+4){
    unsigned char a=bytes[i], b=bytes[i+1], c=bytes[i+2], d=bytes[i+3];
    unsigned long combined = (a << 24) | (b << 16) | (c << 8) | d;
    for (int i=31; i>=0; i--){
    bits[i]=(combined & (1 << i)) ? 1:0;
    }

    //--------------------Decode

    int iv, dest, ope1, ope2, opcode, ivflag;

    dest= (bits[11]<<3) | (bits[10]<<2) | (bits[9]<<1) | (bits[8]);

    ope1 = (bits[19]<<3) | (bits[18]<<2) | (bits[17]<<1) | (bits[16]);

    ivflag=0;

    if (bits[24]==1){
        ivflag=1;
        iv= (bits[7]<<7) | (bits[6]<<6) | (bits[5]<<5) | (bits[4]<<4) | (bits[3]<<3) | (bits[2]<<2) | (bits[1]<<1) | (bits[0]);
    }
    

    else{
        ope2 = (bits[15]<<3) | (bits[14]<<2) | (bits[13]<<1) | (bits[12]);
    }

    opcode = (bits[23]<<3) | (bits[22]<<2) | (bits[21]<<1) | (bits[20]);

    //------------------------------ Execute


    if (opcode==0){ // AND
        if (ivflag==0){
            registers[dest]=registers[ope1] & registers[ope2];
        }
        else{
            registers[dest]=registers[ope1] & iv;
        }
    }

    if (opcode==3){     // ADD
        if (ivflag==0){
        registers[dest]=registers[ope1] + registers[ope2];
        }
        else {
            registers[dest]=registers[ope1] + iv;
        }
    }

    if (opcode==6){     // SUB
        if (ivflag==0){
            registers[dest]=registers[ope1]-registers[ope2];
        }
        else{
            registers[dest]=registers[ope1]-iv;
        }
    }

    
    
    }
}


int main()
{
    FILE *fp;
    unsigned char *data;
    int size;
    int i;

    unsigned long int registers[16];

    for (int r=0; r<16; r++){
        registers[r]=0;
        if (r==5){registers[r]=5;}
        if (r==8){registers[r]=7;}
        if (r==13){registers[r]=25;}
    }


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

    decode(data, size, registers);

    // Clean up
    fclose(fp);
    free(data);

    printf("\n");

    for (int r=0; r<16; r++){
        printf("r%d : %lu", r, registers[r]);
        printf(" | ");
    }

    printf("\n");

    return 0;
}