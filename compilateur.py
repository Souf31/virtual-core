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
    "SUB": 6,       #Dictionnaires d'opcodes / registres / branch codes
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




def createInstruction_opcode(iv, dest, ope1, ope2, opcode, ivflag): # Fonction qui déplace avec des bitwise operators les bonnes valeurs puis pack en binaire en unsigned big endian.                      
    var = iv << 0 | dest << 8 | ope2 << 12 | ope1 << 16 | opcode << 20 | ivflag << 24 | 0 << 25 | 0 << 28
    var = struct.pack('>I', var)                            
    print("Instruction created! " + var.hex())
    return var

def createInstruction_branch(bcc, offset): # De même pour les branch codes
    if offset < 0:
        flag = 1
        offset = abs(offset)

    elif offset > 0:
        flag = 0
        offset = abs(offset)

    if offset > 134217727:
        return("Offset too big")
    
    var = offset << 0 | flag << 27 | bcc << 28
    var = struct.pack('>I', var)
    print("Instruction created! " + var.hex())
    return var


def main(file):
    filename = open(file,"r")
    binname = file[:-2] + ".bin"
    retrievedLines = filename.readlines()       # lis les instructions 
    filename.close()
    binarycode = open(binname,"wb")
    for i in range(len(retrievedLines)):
        retrievedLines[i] = retrievedLines[i].rstrip("\n")
        parsedLines = retrievedLines[i].replace(",", "").split()
        if parsedLines[0] in dict_opcodes:
            opcode = parsedLines[0]
            if dict_opcodes[opcode] not in [5, 8]: # si l'opcodes n'est pas MOV ou CMP : 
                if parsedLines[3].isnumeric():

                    #                                           IV                        DEST                                          op1                     op2            opcode                    IVflag
                    instruction = createInstruction_opcode(int(parsedLines[3]), dict_registers[parsedLines[1].lower()], dict_registers[parsedLines[2].lower()], 0, dict_opcodes[parsedLines[0].upper()], 1)
                else:
                    instruction = createInstruction_opcode(0, dict_registers[parsedLines[1].lower()], dict_registers[parsedLines[2].lower()], dict_registers[parsedLines[3].lower()], dict_opcodes[parsedLines[0].upper()], 0)
            #CMP
            elif dict_opcodes[opcode] == 5:
                if parsedLines[2].isnumeric():
                    instruction = createInstruction_opcode(int(parsedLines[2]), 0, dict_registers[parsedLines[1].lower()], 0, dict_opcodes[parsedLines[0].upper()], 1)
                else:
                    instruction = createInstruction_opcode(0, 0, dict_registers[parsedLines[1].lower()], dict_registers[parsedLines[2].lower()], dict_opcodes[parsedLines[0].upper()], 0)
                
        #MOV r7, r9
            elif dict_opcodes[opcode] == 8:
                if parsedLines[2].isnumeric():
                    instruction = createInstruction_opcode(int(parsedLines[2]), dict_registers[parsedLines[1].lower()], 0, 0, dict_opcodes[parsedLines[0].upper()], 1)
                else:
                    instruction = createInstruction_opcode(0, dict_registers[parsedLines[1].lower()], 0, dict_registers[parsedLines[2].lower()], dict_opcodes[parsedLines[0].upper()], 0)

        elif parsedLines[0] in dict_comp:
            bcc = parsedLines[0]
            instruction = createInstruction_branch(dict_comp[bcc], int(parsedLines[1]))
        else:
            print(parsedLines)
            print("ASM File not valid !")
            return 0

        binarycode.write(instruction)


main(sys.argv[1])
