#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma once

template <class T> 
void heapsort_posun(T a[], int hranica, int korenn) {
	int najvacsi;
	T vymen;

	while(korenn*2 <= hranica) {
		if(korenn*2 == hranica) {
			najvacsi = korenn*2;
		} else if (a[korenn*2] > a[korenn*2 + 1]) {
			najvacsi = korenn*2;
		} else {
			najvacsi = korenn*2 +   1;
		}
		if(a[korenn] < a[najvacsi]) {
			vymen = a[korenn];// vymen
			a[korenn] = a[najvacsi];// vymen
			a[najvacsi] = vymen;
			korenn = najvacsi;
		} else { 
			return; 
		}
	}
}

template <class T> 
void heapsort(T a[], int n) 
{
	int index, pocet;
	T vymen;
	pocet = n-1;
	for(index=(n/2); index>-1; index--) heapsort_posun(a, pocet, index);

	for(index=pocet; index>0; index--) {
		vymen = a[0];
		a[0] = a[index];
		a[index] = vymen;
		heapsort_posun(a, index-1, 0);
	}
}

/*
int main() {
	int i;
	clock_t timer;
	timer = clock();

	for(i=0; i < NN;i++) {
		gA[i]= NN - i;
	}

	timer = clock() - timer;
	printf("%.15f sec\n", (double) timer / (double) CLOCKS_PER_SEC);

	utried(NN);

	timer = clock() - timer;
	printf("%.15f sec\n", (double) timer / (double) CLOCKS_PER_SEC);
	
	return 0;
}

Heapsort:
0 az 20000 trva 0.056 - zrejme pre instrukcie
1M rovnake cisla 0.0750
1M uplne opacne zoradene 0.390
1M dobre zoradene 0.41
100M rovnake 2.0
100M opacne 47
100M dobre 47
Poznamka, lepsie sa vylepsit uz neda.
2,5M za 1 sec

*/