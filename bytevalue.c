#include <stdio.h>
#include <stdlib.h>

void decode() {
    int byte[31];
    unsigned char a=0, b=53, c=129, d=0;
    unsigned long combined = (a << 24) | (b << 16) | (c << 8) | d;
    for (int i=31; i>=0; i--){
    byte[i]=(combined & (1 << i)) ? 1:0;
    printf("%d",byte[i]);
    printf("|");}
    printf("\n");
}

void translate() {
    int bits[31];

    for (int i=0; i<11; i=i+4){

    unsigned long combined = (bits[i]<<3) | (bits[i+1]<<2) | (bits[i+2]<<1) | (bits[i+3]);

    printf("%lu", combined); 
    printf("\n");
    
    }
}

int main(){
    decode();
}