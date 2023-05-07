#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct instruction_t
{
    uint32_t raw_instr;
    int opcode;
    int dest;
    int op1;
    int op2;
    int iv_flag;
    int iv;
    int num_instr;
    int bcc;
    int offset;
    int flag_offset;
} instruction_t;

typedef struct core_t
{
    int pc;
    uint64_t registers[16];
    int carry_flag;
    int branch_flag[6];
} core_t;

uint32_t little_to_big_endian(uint32_t x)
{
    return ((x >> 24) & 0xff) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | ((x << 24) & 0xff000000);
}

void initialize_registers(core_t *core, const char *filename)
{
    FILE *fp;
    char line[32];

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        return;
    }

    while (fgets(line, sizeof(line), fp))
    {
        int reg_num;
        uint32_t value;

        if (sscanf(line, "R%d=0x%x", &reg_num, &value) != 2)
        {
            fprintf(stderr, "Error: invalid line format in file %s\n", filename);
            continue;
        }

        if (reg_num < 0 || reg_num > 15)
        {
            fprintf(stderr, "Error: invalid register number %d in file %s\n", reg_num, filename);
            continue;
        }

        core->registers[reg_num] = value;
    }

    fclose(fp);
}

void fetch(core_t *core, instruction_t **instr_array, const char *filename)
{
    // Open the binary file containing the instructions
    FILE *bin_file = fopen(filename, "rb");

    // Check if the file was opened successfully
    if (!bin_file)
    {
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

    while (fread(&code, sizeof(code), 1, bin_file) == 1)
    {

        // Little to Big endian code
        uint32_t big_code = little_to_big_endian(code);

        // Set the raw instruction
        instructions[i].raw_instr = big_code;

        instructions[i].num_instr = num_instructions;

        // Decode the instruction
        // decode(&instructions[i]);
        printf("Instruction number : %d | Instruction : 0x%08x\n", i, big_code); // use %x to print in hex

        // Increment the instruction count
        i++;
    }
    printf("\n");

    // Set the instruction count and array pointer
    *instr_array = instructions;

    // Close the binary file
    fclose(bin_file);
}

void decode(core_t *core, instruction_t *instr_array)
{
    for (int i = 0; i < instr_array[0].num_instr; i++)
    {
        uint32_t instr = instr_array[i].raw_instr;
        instr_array[i].iv_flag = (instr >> 24) & 0x1;
        instr_array[i].opcode = (instr >> 20) & 0xF;
        instr_array[i].op1 = (instr >> 16) & 0xF;
        instr_array[i].op2 = (instr >> 12) & 0xF;
        instr_array[i].dest = (instr >> 8) & 0xF;
        instr_array[i].iv = instr & 0xFF;
        instr_array[i].offset = instr & 0x07FFFFFF;             // extract the offset from bits 0 to 26
        instr_array[i].flag_offset = (instr & 0x08000000) != 0; // extract the positive/negative flag from bit 27
        instr_array[i].bcc = (instr >> 28) & 0xF;               // extract the BCC code from bits 28 to 31
    }
}

void execute(core_t *core, instruction_t *instr_array)
{

    // Set the PC to zero
    core->pc = 0;
    // printf("PC : %d\n\n", core->pc);

    for (core->pc; core->pc < instr_array[0].num_instr; core->pc++)
    {
        instruction_t instr = instr_array[core->pc];
        switch (instr.opcode)
        {
        case 0: // Logical AND
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] & instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] & core->registers[instr.op2];
            }
            break;
        case 1: // Logical OR
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] | instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] | core->registers[instr.op2];
            }
            break;
        case 2: // Logical XOR
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] ^ instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] ^ core->registers[instr.op2];
            }
            break;
        case 3: // Addition
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] + instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] + core->registers[instr.op2];
            }
            break;
        case 4: // Addition with carry
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] + instr.iv + core->carry_flag;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] + core->registers[instr.op2] + core->carry_flag;
            }
            break;
        case 5: // Comparison
            // do something for CMP
            // TO-DO : Check next instruction if branch; check branch tag; change i with offset

            instruction_t next_instr = instr_array[core->pc + 1];
            //printf("Offset : %d, bool: %d, bcc : %d\n\n", next_instr.offset, next_instr.flag_offset, next_instr.bcc);

            switch (next_instr.bcc)
            {
            case 8: // Unconditional branch B
                if (next_instr.flag_offset == 0)
                {
                    core->pc = core->pc + next_instr.offset;
                }
                if (next_instr.flag_offset == 1)
                {
                    core->pc = core->pc - next_instr.offset;
                }
                break;
            case 9: // Branch BEQ
                if (next_instr.flag_offset == 0)
                {
                    if (core->registers[instr.op1] == core->registers[instr.op2])
                    {
                        core->pc = core->pc + next_instr.offset;
                    }
                }
                if (next_instr.flag_offset == 1)
                {
                    if (core->registers[instr.op1] == core->registers[instr.op2])
                    {
                        core->pc = core->pc - next_instr.offset;
                    }
                }
                break;
            case 10: // Branch BNE
                if (next_instr.flag_offset == 0)
                {
                    if (core->registers[instr.op1] != core->registers[instr.op2])
                    {
                        core->pc = core->pc + next_instr.offset;
                    }
                }
                if (next_instr.flag_offset == 1)
                {
                    if (core->registers[instr.op1] != core->registers[instr.op2])
                    {
                        core->pc = core->pc - next_instr.offset;
                    }
                }
                break;
            case 11: // Branch BLE
                if (next_instr.flag_offset == 0)
                {
                    if (core->registers[instr.op1] <= core->registers[instr.op2])
                    {
                        core->pc = core->pc + next_instr.offset;
                    }
                }
                if (next_instr.flag_offset == 1)
                {
                    if (core->registers[instr.op1] <= core->registers[instr.op2])
                    {
                        core->pc = core->pc - next_instr.offset;
                    }
                }
                break;
            case 12: // Branch BGE
                if (next_instr.flag_offset == 0)
                {
                    if (core->registers[instr.op1] >= core->registers[instr.op2])
                    {
                        core->pc = core->pc + next_instr.offset;
                    }
                }
                if (next_instr.flag_offset == 1)
                {
                    if (core->registers[instr.op1] >= core->registers[instr.op2])
                    {
                        core->pc = core->pc - next_instr.offset;
                    }
                }
                break;
            case 13: // Branch BL
                if (next_instr.flag_offset == 0)
                {
                    if (core->registers[instr.op1] < core->registers[instr.op2])
                    {
                        core->pc = core->pc + next_instr.offset;
                    }
                }
                if (next_instr.flag_offset == 1)
                {
                    if (core->registers[instr.op1] < core->registers[instr.op2])
                    {
                        core->pc = core->pc - next_instr.offset;
                    }
                }
                break;
            case 14: // Branch BG
                if (next_instr.flag_offset == 0)
                {
                    if (core->registers[instr.op1] > core->registers[instr.op2])
                    {
                        core->pc = core->pc + next_instr.offset;
                    }
                }
                if (next_instr.flag_offset == 1)
                {
                    if (core->registers[instr.op1] > core->registers[instr.op2])
                    {
                        core->pc = core->pc - next_instr.offset;
                    }
                }
            }
            break;

            break;
        case 6: // Subtraction
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] - instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] - core->registers[instr.op2];
            }
            break;
        case 7: // Subtraction with carry
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] - instr.iv - !core->carry_flag;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] - core->registers[instr.op2] - !core->carry_flag;
            }
            break;
        case 8: // Move data
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op2];
            }
            break;
        case 9: // Logical left shift
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] << instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] << core->registers[instr.op2];
            }
            break;
        case 10: // Logical right shift
            if (instr.iv_flag)
            {
                core->registers[instr.dest] = core->registers[instr.op1] >> instr.iv;
            }
            else
            {
                core->registers[instr.dest] = core->registers[instr.op1] >> core->registers[instr.op2];
            }
            break;
        default:
            printf("Error! Incorrect instruction.\n");
            exit(-1);
            break;
        }
    }
}

int main(int argc, char *argv[]){

    // unsigned long long int maxvalue;

    // maxvalue = 18446744073709551615;

    // printf("\nTest : %llu\n", maxvalue);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *binfile = argv[1];
    char *statefile = argv[2];

    // printf("Bin: %s, state: %s\n\n", binfile, statefile);

    int *instruction_count;

    struct instruction_t *instructions;
    struct core_t core;

    initialize_registers(&core, statefile);

    fetch(&core, &instructions, binfile);

    // printf("Instruction number 2 : 0x%08x; instruction count : %d\n", instructions[2].raw_instr, instructions[1].num_instr);

    decode(&core, instructions);

    // printf("Instruction number 3 opcode : %d\n", instructions[3].opcode);

    execute(&core, instructions);

    for (int i = 0; i < 16; i++)
    {
        printf("r[%llu] = %llu\n", i, core.registers[i]);
    }

    return 0;
}