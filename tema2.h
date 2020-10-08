#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef __TEMA2_H__
#define __TEMA2_H__

typedef struct Tree {
	//valoarea din nodul respectiv
	char **val;

	//primul copil al nodului
	struct Tree *copil;

	//primul frate al nodului de pe acelasi nivel
	struct Tree *frateNod;
} Tree;

void createRoot(FILE **sursa, FILE **dest);
void genArbore(Tree *nod, int *dims, char *player);
void copiereMatrice(Tree *nou, Tree *nod, int *dims);
void printareNod(FILE *dest, Tree *nod, int *dims, int tabs);
int verifWinVerticala(Tree *nod, int *dims, char *player);
int	verifWinOrizontala(Tree *nod, int *dims, char *player);
int verifWinDiagonalaP(Tree *nod, int *dims, char *player);
int verifWinDiagonalaS(Tree *nod, int *dims, char *player);
int verificareWIN(Tree *nod, int *dims, char *player);
void dealocareMem(Tree *nod, int *dims);

#endif /* __TEMA2_H__ */
