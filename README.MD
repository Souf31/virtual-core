# Virtual Core

## Structure

### Coeur

Le core prends en entrée un binaire ainsi qu'un fichier d'état intial des registres et execute les opérations récupérées. Ce fichier binaire est crée par un compilateur qui prendra en entrée un fichier d'instructions écrit en assembleur like.

Le coeur doit être dans un langage compilé, donc j'ai décidé de le faire en C.

### Compilateur

Le compilateur prends en entrée un fichier d'instruction assembleur like, et doit pouvoir décoder ces instructions, et créer un fichier binaire de ces instructions. Le compilateur sera en python.

ADD r1, r5, r8

0000 0000 0000 0011 0101 1000 0001 0000

0000 0000 0011 0101 1000 0001 0000 0000

CMP r8, 18

0000 0001 0101 1000 0000 0000 0001 0010

SUB r1, r5, 4

0000 0001 0110 0101 0000 0001 0000 0100

MOV r7, r9

0000 0000 1000 1001 0000 0111 0000 0000