//
// Created by Vladislav Volkov on 29.02.2024.
//

#ifndef ECO_LAB1_ECOLAB1TESTDOUBLE_H
#define ECO_LAB1_ECOLAB1TESTDOUBLE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <IEcoMemoryAllocator1.h>
#include "IEcoLab1.h"

void *CreateRootsDoubleArray(IEcoMemoryAllocator1* pIMem, size_t n_roots) {
    double *out = (double*) pIMem->pVTbl->Alloc(pIMem, n_roots * sizeof(double));
	size_t i;
    srand(time(0));
    for (i = 0; i < n_roots; ++i) {
        double range = 20.0;
        out[i] = ((double)rand() / (double)RAND_MAX) * range - 10.0;
    }
    return out;
}

void DeleteDoubleArray(IEcoMemoryAllocator1* pIMem, void* array_ptr) {
    pIMem->pVTbl->Free(pIMem, array_ptr);
}

void PrintDoubleArray(void* array, size_t n_elems) {
    double* result = (double*) array;
	size_t i;
    for (i = 0; i < n_elems; ++i) {
        printf("%f ", *(result + i));
    }
    printf("\n");
}

int InitDoubleOnes(void* elem) {
    *((double*)elem) = 1;
    return 0;
}

int MultDouble(const void* first, const void* second, void* result) {
    *((double*)result) =  (*((double*)first)) * (*((double*)second));
    return 0;
}

int SubDouble(const void* first, const void* second, void* result) {
    *((double*)result) =  (*((double*)first)) - (*((double*)second));
    return 0;
}

void PrintDoubleTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
	void* roots_ptr;
	void* coefs;
    printf("\n\nTestDouble\n");
    roots_ptr = CreateRootsDoubleArray(pIMem, n_roots);
    printf("Roots:\n");
    PrintDoubleArray(roots_ptr, n_roots);
    coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(double));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(double), MultDouble, SubDouble, InitDoubleOnes, coefs);
    printf("Coefficients:\n");
    PrintDoubleArray(coefs, n_roots + 1);
    DeleteDoubleArray(pIMem, roots_ptr);
}

void WriteToFileDoubleTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
	errno_t code;
	void* roots_ptr;
	void* coefs_ptr;
	double* coefficients;
	double* roots;
	size_t i;
    sprintf(csv_name, "DoubleResults_%d.csv", n_roots);
    code = fopen_s(&file, csv_name, "w");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    roots_ptr = CreateRootsDoubleArray(pIMem, n_roots);
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(double));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(double), MultDouble, SubDouble, InitDoubleOnes, coefs_ptr);
    fprintf(file, "%s,%s\n", "Roots", "Coefficients");
    coefficients = (double*) coefs_ptr;
    roots = (double*) roots_ptr;
    for (i = 0; i < n_roots; ++i) {
        if (i == 0) {
            fprintf(file, "%lf,%lf\n", roots[i], coefficients[i]);
            continue;
        }
        fprintf(file, "%lf,%lf\n", roots[i], coefficients[i]);
    }
    fprintf(file, " ,%lf\n", coefficients[n_roots]);
    fclose(file);
    DeleteDoubleArray(pIMem, roots_ptr);
}

void WriteTimeToFileDouble(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
	errno_t code;
	void* roots_ptr;
	void* coefs_ptr;
	clock_t begin, end;
	double work_time;
    sprintf(csv_name, "TimeDouble.csv");
    code = fopen_s(&file, csv_name, "a");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    roots_ptr = CreateRootsDoubleArray(pIMem, n_roots);
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(int));
    begin = clock();
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(int), MultDouble, SubDouble, InitDoubleOnes, coefs_ptr);
    end = clock();
    work_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    fprintf(file, "%d,%f\n", n_roots, work_time);
    fclose(file);
    DeleteDoubleArray(pIMem, roots_ptr);
}

#endif //ECO_LAB1_ECOLAB1TESTDOUBLE_H
