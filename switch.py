iv=0
dest=7
ope1=9
ope2=0
opcode=8
ivflag=0


var = iv << 0 | dest << 8 | ope2 << 12 | ope1 << 16 | opcode << 20 | ivflag << 24 | 0 << 25 | 0 << 28

print(var)
print(format(var, '#034b'))