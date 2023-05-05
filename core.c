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
    int num_instr;
} instruction_t;

typedef struct core_t {
    uint32_t pc;
    uint32_t registers[16];
    uint32_t carry_flag;
    uint32_t branch_flag[6];
} core_t;

uint32_t little_to_big_endian(uint32_t x) {
    return ((x >> 24) & 0xff) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | ((x << 24) & 0xff000000);
}

void fetch(core_t *core, instruction_t **instr_array) {
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

        //Little to Big endian code
        uint32_t big_code = little_to_big_endian(code);

        // Set the raw instruction
        instructions[i].raw_instr = big_code;

        instructions[i].num_instr = num_instructions;

        // Decode the instruction
        //decode(&instructions[i]);
        printf("Instruction number : %d | Instruction : 0x%08x\n", i, big_code); // use %x to print in hex

        // Increment the instruction count
        i++;
    }

    // Set the instruction count and array pointer
    *instr_array = instructions;

    

    // Close the binary file
    fclose(bin_file);
}

void decode(core_t *core, instruction_t *instr_array){
    for (int i = 0; i < instr_array[0].num_instr; i++) {

        uint32_t instr = instr_array[i].raw_instr;
        instr_array[i].iv_flag = (instr >> 24) & 0x1;
        instr_array[i].opcode = (instr >> 20) & 0xF;
        instr_array[i].op1 = (instr >> 16) & 0xF;
        instr_array[i].op2 = (instr >> 12) & 0xF;
        instr_array[i].dest = (instr >> 8) & 0xF;
        instr_array[i].iv = instr & 0xFF;

        printf("%d\n", instr_array[i].opcode);

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

    fetch(&core, &instructions);

    // printf("Instruction number 2 : 0x%08x; instruction count : %d\n", instructions[2].raw_instr, instructions[1].num_instr);

    decode(&core, instructions);

    // printf("Instruction number 3 opcode : %d\n", instructions[3].opcode);

    return 0;
}