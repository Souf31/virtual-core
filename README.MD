# Virtual Core

## Structure

### Coeur

Le core prends en entrée un binaire ainsi qu'un fichier d'état intial des registres et execute les opérations récupérées. Ce fichier binaire est crée par un compilateur qui prendra en entrée un fichier d'instructions écrit en assembleur like.

Le coeur doit être dans un langage compilé, donc j'ai décidé de le faire en C.

### Compilateur

Le compilateur prends en entrée un fichier d'instruction assembleur like, et doit pouvoir décoder ces instructions, et créer un fichier binaire de ces instructions. Le compilateur sera en python.

### Fonctionnement du coeur

#### Fetch

La fonction fetch() permet de récupérer les instructions stockées dans le fichier binaire.

#### Decode

La fonction decode() s'ocucupe de remplir chaque champ de la structure instruction en analysant les 32 bits.

#### Execute

La fonction execute() quant à elle effectue les instructions sur les registres avec les informations récupérées au préalable.

### Utilisation

Pour faire fonctionner le coeur, utilisez le Makefile. Entrez 
``` make run```, ```make run_128``` ou ```make run_lshift```