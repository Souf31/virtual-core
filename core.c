#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct instruction_t{
    uint32_t raw_instr;
    uint32_t opcode;
    uint32_t dest;
    uint32_t op1;
    uint32_t op2;
    uint32_t iv_flag;
    uint32_t iv;
    unsigned char *instructions
} instruction_t;

typedef struct core_t {
    uint32_t pc;
    uint32_t registers[16];
    uint32_t carry_flag;
    uint32_t branch_flag[6];
} core_t;

void fetch(core_t *core, instruction_t **instr_array, int *instr_count) {
    // Open the binary file containing the instructions
    FILE *bin_file = fopen("binarycode.bin", "rb");

    // Check if the file was opened successfully
    if (!bin_file) {
        perror("Unable to open binary file");
        exit(EXIT_FAILURE);
    }

    // Get the file size
    fseek(bin_file, 0L, SEEK_END);
    long int file_size = ftell(bin_file);
    rewind(bin_file);

    // Allocate memory to store the instructions
    int num_instructions = file_size / sizeof(uint32_t);
    instruction_t *instructions = malloc(num_instructions * sizeof(instruction_t));

    // Read the instructions from the binary file
    uint32_t code;
    int i = 0;
    while (fread(&code, sizeof(code), 1, bin_file) == 1) {
        // Set the raw instruction
        instructions[i].raw_instr = code;

        // Decode the instruction
        //decode(&instructions[i]);
        printf("Instruction number : %d\n", i);

        // Increment the instruction count
        i++;
    }

    // Set the instruction count and array pointer
    *instr_count = num_instructions;
    *instr_array = instructions;

    // Close the binary file
    fclose(bin_file);
}


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


    // for (int r=0; r<16; r++){
    //     registers[r]=0;
    //     if (r==5){registers[r]=5;}
    //     if (r==8){registers[r]=7;}
    //     if (r==13){registers[r]=25;}
    // }

    // Use the data as needed
    // for (i = 0; i < size; i++) {
    //     printf("Data[%d] = ", i);
    //     printf("%d", data[i]);
    //     printf("\n");
    // }

    // decode(data, size, registers);

    int* instruction_count;

    struct instruction_t *instructions;
    struct core_t core;

    fetch(&core, &instructions, instruction_count);

    return 0;
}