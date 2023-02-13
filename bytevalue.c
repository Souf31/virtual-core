#include <stdio.h>
#include <stdlib.h>

void main() {
    int byte[31];
    unsigned char a=0, b=53, c=129, d=0;
    unsigned long combined = (a << 24) | (b << 16) | (c << 8) | d;
    for (int i=31; i>=0; i--){
    byte[i]=(combined & (1 << i)) ? 1:0;
    printf("%d",byte[i]);
    printf("|");}
}