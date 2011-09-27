#include <stdio.h>
#include <stdlib.h>
#include "mysum.h"

int sum(int a, int b) {
	return a + b;
}

int produit(int a, int b) {
	return a * b;
}

int main(void) {
	char a[80], b[80];
	int c = 0, d = 0;

	do {
		printf("Nb1: ");

	} while(fgets(a, sizeof(a), stdin) != NULL && !(c = atoi(a)));

	do {
		printf("Nb2: ");

	} while(fgets(b, sizeof(b), stdin) != NULL && !(d = atoi(b)));

	printf("Somme: %d\n", sum(c, d));
	printf("Produit: %d\n", produit(c, d));

	return 0;
}
