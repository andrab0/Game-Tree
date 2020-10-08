#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema2.h"

int main(int argc, char **argv)
{
	FILE *sursa, *dest;
	char cerinta1[4] = "-c1"
		, cerinta2[4] = "-c2"
		, cerinta3[4] = "-c3"
		, bonus[4] = "-b";
	char cerinta[4];

	//deschiderea fisierului destinatie
	dest = fopen(argv[3], "w");
	if (!dest) {
		fprintf(stderr, "%s\n",
			"ERROARE! Fisierul destinatie nu poate fi deschis.");
		return 0;
	}

	//deschiderea fisierului sursa
	sursa = fopen(argv[2], "r");
	if (!sursa) {
		fprintf(stderr, "%s\n",
			"ERROARE! Fisierul sursa nu poate fi deschis.");
		return 0;
	}

	//extragerea numelui cerintei
	strcpy(cerinta, argv[1]);

	//verificarea cerintei transmise ca parametru
	if (strcmp(cerinta1, cerinta) == 0) {
		createRoot(&sursa, &dest);
	} else {
		if (strcmp(cerinta2, cerinta) == 0)
			fprintf(dest, "%s\n", "se rezolva cerinta2.");
		else {
			if (strcmp(cerinta3, cerinta) == 0)
				fprintf(dest, "%s\n", "se rezolva cerinta3.");
			else {
				if (strcmp(bonus, cerinta) == 0)
					fprintf(dest, "%s\n", "bonus.");
			}
		}
	}

	//inchiderea fisierelor transmise ca parametrii
	fclose(sursa);
	fclose(dest);
}

void createRoot(FILE **sursa, FILE **dest)
{
	Tree *root;
	char player[2];
	int *dims;
	int i, j;

	//citirea dimensiunilor tablei de joc
	dims = (int *)malloc(sizeof(int) * 2);
	for (i = 0; i < 2; i++)
		fscanf(*sursa, "%d ", &dims[i]);


	//citirea jucatorului de pe pozitia "MAX"
	fscanf(*sursa, "%c\n", player);

	//alocare memorie pentru matricea tablei de joc
	root = (Tree *)malloc(sizeof(Tree));
	root->val = (char **)malloc(sizeof(char *) * dims[0]);
	for (i = 0; i < dims[0] ; i++)
		root->val[i] = (char *)malloc(sizeof(char) * (dims[1]));

	root->copil = NULL;
	root->frateNod = NULL;

	//citirea tablei de joc din fisier
	for (i = 0; i < dims[0]; i++) {
		for (j = 0; j < dims[1]; j++) {
			fscanf(*sursa, "%c", &(root->val[i][j]));
			getc(*sursa);
		}
	}

	//generarea posibilitatilor de joc
	genArbore(root, dims, player);

	//printarea tuturor posibilitatilor de joc
	printareNod(*dest, root, dims, 0);

	//eliberarea recursiva a memoriei utilizate pentru
	//construirea arborelui de joc
	dealocareMem(root, dims);

	//eliberarea memoriei folosite pentru retinerea dimensiunilor
	//tablei de joc
	free(dims);
}

void genArbore(Tree *nod, int *dims, char *player)
{
	int i, j, k, ok, nuEwin;
	Tree *temp, *p;

	p = nod;

	temp = (Tree *)malloc(sizeof(Tree));
	temp->val = (char **)malloc(sizeof(char *) * dims[0]);
	for (i = 0; i < dims[0]; i++)
		temp->val[i] = (char *)malloc(sizeof(char) * dims[1]);

	temp->copil = NULL;
	temp->frateNod = NULL;

	//salvarea matricii nodului intr-o variabila auxiliara
	copiereMatrice(temp, p, dims);

	for (j = 0; j < dims[1]; j++) {
		ok = 0;
		for (i = dims[0] - 1; i >= 0; i--) {
			if (temp->val[i][j] == '-') {
				ok++;
				if (ok == 1) {
					if (nod->copil == NULL) {

						//alocare memorie pentru nodul copil
						//si pentru matricea sa
						p->copil = (Tree *)malloc(sizeof(Tree));
						p->copil->val =
						(char **)malloc(sizeof(char *) * dims[0]);
						for (k = 0; k < dims[0]; k++)
							p->copil->val[k] =
							(char *)malloc(sizeof(char) * dims[1]);

						//initializarea matricei copilului nodului
						copiereMatrice(p->copil, temp, dims);

						//adaugarea valorii player-ului
						p->copil->val[i][j] = player[0];

						p = p->copil;
						p->copil = NULL;
						p->frateNod = NULL;

						//verificare conditii win
						nuEwin = verificareWIN(p, dims, player);

						if (nuEwin == 0) {
							if (player[0] == 'R')
								genArbore(p, dims, "B");
							else
								if (player[0] == 'B')
									genArbore(p, dims, "R");
						} else
							break;
					} else {

						//alocare memorie pentru nodul frate
						//si pentru matricea sa
						p->frateNod = (Tree *)malloc(sizeof(Tree));
						p->frateNod->val =
						(char **)malloc(sizeof(char *) * dims[0]);
						for (k = 0; k < dims[0]; k++)
							p->frateNod->val[k] =
							(char *)malloc(sizeof(char) * dims[1]);

						//initializare matrice frate
						copiereMatrice(p->frateNod, temp, dims);

						//adaugarea valorii player-ului
						p->frateNod->val[i][j] = player[0];

						p = p->frateNod;
						p->frateNod = NULL;
						p->copil = NULL;

						//verificare conditii win
						nuEwin = verificareWIN(p, dims, player);

						if (nuEwin == 0) {
							if (player[0] == 'R')
								genArbore(p, dims, "B");
							else
								if (player[0] == 'B')
									genArbore(p, dims, "R");
						} else
							break;
					}
				} else
					if (ok == 0)
						break;
			}
		}
	}

	//eliberare memorie pentru nodul temporar utilizat
	for (i = 0; i < dims[0]; i++)
		free(temp->val[i]);
	free(temp->val);
	free(temp);
}


//functie pentru copierea matricii unui nod in
//matricea altui nod destinatie
void copiereMatrice(Tree *nou, Tree *nod, int *dims)
{
	int i, j;

	for (i = 0; i < dims[0]; i++) {
		for (j = 0; j < dims[1]; j++)
			nou->val[i][j] = nod->val[i][j];
	}
}

//functie recursiva afisare arbore
void printareNod(FILE *dest, Tree *nod, int *dims, int tabs)
{
	Tree *temp;
	int i, j, k;

	//initializarea unui nod temporar folosit pentru parcurgerea matricii
	temp = nod;

	//printarea unui rand liber intre copii unui nod
	if (tabs != 0)
		fprintf(dest, "\n");

	//verificarea existentei nodului
	while (temp != NULL) {

		//printarea tab-urilor in functie de nivelul pe care se afla nodul
		for (k = 0; k < tabs; k++)
			fprintf(dest, "\t");

		//parcurgerea matricei si afisarea elementelor sale
		for (i = 0; i < dims[0]; i++) {
			for (j = 0; j < dims[1]; j++) {
				if (j == dims[1] - 1) {
					fprintf(dest, "%c", (temp->val[i][j]));
					fprintf(dest, "\n");
					if (i < dims[0] - 1) {
						for (k = 0; k < tabs; k++)
							fprintf(dest, "\t");
					}
				} else {
					fprintf(dest, "%c ", (temp->val[i][j]));
				}
			}
		}

		//reapelarea functiei pentru fiecare nod copil
		//tinand cont de cresterea adancimii
		printareNod(dest, temp->copil, dims, tabs + 1);

		//trecerea la urmatorul nod frate in urma afisarii
		//tuturor copiilor
		temp = temp->frateNod;
	}
}

//functie pentru verificarea conditiei de win pe verticala
int verifWinVerticala(Tree *nod, int *dims, char *player)
{
	int i, j;
	int winV = 0;

	for (i = 0; i < dims[0]; i++) {
		for (j = 0; j < dims[1]; j++) {
			if ((i+3) < dims[0]) {
				if (nod->val[i][j] == player[0] &&
					nod->val[i+1][j] == player[0] &&
					nod->val[i+2][j] == player[0] &&
					nod->val[i+3][j] == player[0])
					winV = 1;
			}
		}
	}
	return winV;
}

//functie pentru verificarea conditiei de win pe orizontala
int verifWinOrizontala(Tree *nod, int *dims, char *player)
{
	int i, j;
	int winO = 0;

	for (i = 0; i < dims[0]; i++) {
		for (j = 0; j < dims[1]; j++) {
			if ((j+3) < dims[1]) {
				if (nod->val[i][j] == player[0] &&
					nod->val[i][j+1] == player[0] &&
					nod->val[i][j+2] == player[0] &&
					nod->val[i][j+3] == player[0])
					winO = 1;
			}
		}
	}
	return winO;
}

//functie pentru verificarea conditiei de win atat pe
//diagonala principala cat si pe subdiagonalele principale
int verifWinDiagonalaP(Tree *nod, int *dims, char *player)
{
	int i, j;
	int winDP = 0;

	for (i = 3; i < dims[0]; i++) {
		for (j = 0; j < dims[1]-3; j++) {
			if (nod->val[i][j] == player[0] &&
				nod->val[i-1][j+1] == player[0] &&
				nod->val[i-2][j+2] == player[0] &&
				nod->val[i-3][j+3] == player[0])
				winDP = 1;
		}
	}
	return winDP;
}

//functie pentru verificarea conditiei de win atat pe
//diagonala secundara cat si pe subdiagonalele secundare
int verifWinDiagonalaS(Tree *nod, int *dims, char *player)
{
	int i, j;
	int winDS = 0;

	for (i = 3; i < dims[0]; i++) {
		for (j = 3; j < dims[1]; j++) {
			if (nod->val[i][j] == player[0] &&
				nod->val[i-1][j-1] == player[0] &&
				nod->val[i-2][j-2] == player[0] &&
				nod->val[i-3][j-3] == player[0])
				winDS = 1;
		}
	}
	return winDS;
}


//functie pentru verificarea simultana a celor 4 cazuri in
//care este afisata o situatie castigatoare
int verificareWIN(Tree *nod, int *dims, char *player)
{
	int winV, winO, winDP, winDS;

	//verificarea verticala
	winV = verifWinVerticala(nod, dims, player);

	//verificarea orizontala
	winO = verifWinOrizontala(nod, dims, player);

	//verificarea diagonalei si a subdiagonalelor principale
	winDP = verifWinDiagonalaP(nod, dims, player);

	//verificarea diagonalei si a subdiagonalelor secundare
	winDS = verifWinDiagonalaS(nod, dims, player);

	if (winV == 1 || winO == 1 || winDP == 1 || winDS == 1)
		return 1;
	else
		return 0;
}

//functie pentru eliberarea recursiva a memoriei
void dealocareMem(Tree *nod, int *dims)
{
	int i;

	//verificarea existentei nodului
	if (nod != NULL) {

		//eliberarea matricei nodului
		for (i = 0; i < dims[0]; i++)
			free(nod->val[i]);

		free(nod->val);

		//reapelarea functiei pentru frate
		dealocareMem(nod->frateNod, dims);

		//reapelarea functiei pentru copilul nodului
		dealocareMem(nod->copil, dims);
		free(nod);
	}
}
