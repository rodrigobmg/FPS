#include <stdlib.h> 
#include <stdio.h> 
#pragma once
#define MIN_MERGESORT_LIST_SIZE    32

template <class T> 
void mergesort(T a[], int size, T temp[]) {
    int i1, i2, tempi;
    if (size < MIN_MERGESORT_LIST_SIZE) {
        /* Use insertion sort */
        int i, j;
		T v;
        for (i=0; i < size; i++) {
			v = a[i];
            for (j = i - 1; j >= 0; j--) {
               if (a[j] <= v) break;
                a[j + 1] = a[j];
            }
            a[j + 1] = v;
        }
        return;
    }

    mergesort(a, size/2, temp);
    mergesort(a + size/2, size - size/2, temp);
    i1 = 0;
    i2 = size/2;
    tempi = 0;
    while (i1 < size/2 && i2 < size) {
        if (a[i1] <= a[i2]) {
            temp[tempi] = a[i1];
            i1++;
        } else {
            temp[tempi] = a[i2];
            i2++;
        }
        tempi++;
    }

    while (i1 < size/2) {
        temp[tempi] = a[i1];
        i1++;
        tempi++;
    }
    while (i2 < size) {
        temp[tempi] = a[i2];
        i2++;
        tempi++;
    }

    memcpy(a, temp, size*sizeof(T));
}

template <class T> 
void mergesort_one(T a[], int size) {
	T* temp = malloc(sizeof(T)*size);
	mergesort(a, size, temp);
	delete[] temp;
}



/*
int main() {
	int i;
	clock_t timer;

    int* a    = malloc(sizeof(int)*NN);
    int* temp = malloc(sizeof(int)*NN);

	if(a == NULL || temp == NULL) {
		return 1;
	}

	timer = clock();

	for(i=0; i < NN;i++) {
		a[i]=  NN - i;
	}

	timer = clock() - timer;
	printf("%.15f sec\n", (double) timer / (double) CLOCKS_PER_SEC);

	mergesort_array(a, NN, temp);

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

Merge sort:
2x viac pamete
1M 0.19 dobre
1M 0.29 opacne
10M 1.7 dobre
10M 1.7 rovnake
10M 2.3 opacne - sposobene insert sortom
100M dobre, rovnake 17.8
100M zle 26.9
1B - nepusti pamet, strankovanie ma velky vplyv
4,5M za 1 sec - len 1 thread !

*/