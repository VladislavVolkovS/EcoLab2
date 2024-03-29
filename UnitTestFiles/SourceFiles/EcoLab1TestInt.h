//
// Created by Vladislav Volkov on 29.02.2024.
//

#ifndef ECO_LAB1_ECOLAB1TESTINT_H
#define ECO_LAB1_ECOLAB1TESTINT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <IEcoMemoryAllocator1.h>
#include "IEcoLab1.h"

void *CreateRootsIntArray(IEcoMemoryAllocator1* pIMem, size_t n_roots) {
    int *out = (int*) pIMem->pVTbl->Alloc(pIMem, n_roots * sizeof(int));
	size_t i;
    srand(time(0));
    for (i = 0; i < n_roots; ++i) {
        out[i] = (int)rand() % 20 - 10;
    }
    return out;
}

void DeleteIntArray(IEcoMemoryAllocator1* pIMem, void* array_ptr) {
    pIMem->pVTbl->Free(pIMem, array_ptr);
}

void PrintIntArray(void* array, size_t n_elems) {
    int* result = (int*) array;
	size_t i;
    for (i = 0; i < n_elems; ++i) {
        printf("%d ", *(result + i));
    }
    printf("\n");
}

int InitIntOnes(void* elem) {
    *((int*)elem) = 1;
    return 0;
}

int MultInt(const void* first, const void* second, void* result) {
    *((int*)result) =  (*((int*)first)) * (*((int*)second));
    return 0;
}

int SubInt(const void* first, const void* second, void* result) {
    *((int*)result) =  (*((int*)first)) - (*((int*)second));
    return 0;
}

void PrintIntTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    void* roots_ptr = CreateRootsIntArray(pIMem, n_roots);
	void* coefs;
	printf("\n\nTestInt\n");
    printf("Roots:\n");
    PrintIntArray(roots_ptr, n_roots);
    coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(int));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(int), MultInt, SubInt, InitIntOnes, coefs);
    printf("Coefficients:\n");
    PrintIntArray(coefs, n_roots + 1);
    DeleteIntArray(pIMem, roots_ptr);
}

void WriteToFileIntTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
	void* roots_ptr;
	void* coefs_ptr;
	int* coefficients;
	int* roots;
	size_t i;
	errno_t code;
    sprintf(csv_name, "IntResults_%d.csv", n_roots);
    code = fopen_s(&file, csv_name, "w");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    roots_ptr = CreateRootsIntArray(pIMem, n_roots);
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(int));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(int), MultInt, SubInt, InitIntOnes, coefs_ptr);
    fprintf(file, "%s,%s\n", "Roots", "Coefficients");
    coefficients = (int*) coefs_ptr;
    roots = (int*) roots_ptr;
    for (i = 0; i < n_roots; ++i) {
        if (i == 0) {
            fprintf(file, "%d,%d\n", roots[i], coefficients[i]);
            continue;
        }
        fprintf(file, "%d,%d\n", roots[i], coefficients[i]);
    }
    fprintf(file, " ,%d\n", coefficients[n_roots]);
    fclose(file);
    DeleteIntArray(pIMem, roots_ptr);
}

void WriteTimeToFileInt(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
	errno_t code;
	void* roots_ptr;
	void* coefs_ptr;
	double work_time;
	clock_t begin, end;
    sprintf(csv_name, "TimeInt.csv");
    code = fopen_s(&file, csv_name, "a");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    roots_ptr = CreateRootsIntArray(pIMem, n_roots);
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(int));
    begin = clock();
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(int), MultInt, SubInt, InitIntOnes, coefs_ptr);
    end = clock();
    work_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    fprintf(file, "%d,%f\n", n_roots, work_time);
    fclose(file);
    DeleteIntArray(pIMem, roots_ptr);
}

#endif //ECO_LAB1_ECOLAB1TESTINT_H
