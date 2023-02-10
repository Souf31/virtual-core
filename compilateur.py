import re
import sys
import struct

dict_opcodes = {
    "AND": 0,
    "ORR": 1,
    "EOR": 2,
    "ADD": 3,
    "ADC": 4,
    "CMP": 5,
    "SUB": 6,
    "SBC": 7,
    "MOV": 8,
    "LSH": 9,
    "RSH": 10
}


dict_registers = {
    "r0": 0,
    "r1": 1,
    "r2": 2,
    "r3": 3,
    "r4": 4,
    "r5": 5,
    "r6": 6,
    "r7": 7,
    "r8": 8,
    "r9": 9,
    "r10": 10,
    "r11": 11,
    "r12": 12,
    "r13": 13,
    "r14": 14,
    "r15": 15
}

dict_comp = {
    "B": 8,
    "BEQ": 9,
    "BNE": 10,
    "BLE": 11,
    "BGE": 12,
    "BL": 13,
    "BG": 14,
}

# 0  1      5     8     3       0                                       0    0    0   3     5    8    1    0


# ADD r1, r5, r8 | 0000 0000 0000 0011 0101 1000 0001 0000
def createInstruction_opcode(iv, dest, ope1, ope2, opcode, ivflag):
    var = (iv << 0) | dest << 8 | ope1 << 12 | ope2 << 16 | opcode << 20 | ivflag << 24 | 0 << 28 | 0 << 31
    var = struct.pack('>i', var)
    print("Instruction created! \n")
    print(var.hex())
    return var


def main(file):
    with open(file, "r") as filebinary:
        retrievedLines = file.readlines()
    file.close()
    with open("code_testadd", 'wb') as file:
        for i in range(len(retrievedLines)):
            retrievedLines[i] = retrievedLines[i].rstrip("\n")
            parsedLines = retrievedLines[i].replace(",", "").split()
            if parsedLines[0] in dict_opcodes:
                opcode = parsedLines[0]
                if opcode not in [5, 8]:
                    print("Proceeding: \n")

                    if parsedLines[3].isnumeric():
                        instruction = createInstruction_opcode(parsedLines[3], dict_registers[parsedLines[1].lower()], dict_registers[parsedLines[2].lower()], 0, dict_opcodes[parsedLines[0].upper()], 1)
                    else:
                        instruction = createInstruction_opcode(0, dict_registers[parsedLines[1].lower()], dict_registers[parsedLines[2].lower()], dict_registers[parsedLines[3].lower()], dict_opcodes[parsedLines[0].upper()], 0)

                    filebinary.write(instruction)

                else:
                    print("Not implemented yet !")
                    return 0

            elif parsedLines[0] in dict_comp:
                opcode = parsedLines[0]

            else:
                print("ASM File not valid !")
                return 0


main(sys.argv[1])
