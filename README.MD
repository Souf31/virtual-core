# Virtual Core

## Structure

### Coeur

Le core prends en entrée un binaire ainsi qu'un fichier d'état intial des registres et execute les opérations récupérées. Ce fichier binaire est crée par un compilateur qui prendra en entrée un fichier d'instructions écrit en assembleur like.

Le coeur doit être dans un langage compilé, donc j'ai décidé de le faire en C.

### Compilateur

Le compilateur prends en entrée un fichier d'instruction assembleur like, et doit pouvoir décoder ces instructions, et créer un fichier binaire de ces instructions. Le compilateur sera en python.
