import struct

a = 0
b= 1
c= 5
d= 8
e= 3
f= 0

print(bin(a))
print(bin(b))
var = (a << 0) | b<<8 | c<<12 | d<<16 | e<<20 | f<<24 | 0<<28 | 0<<31 

print(var)
print(bin(var))
