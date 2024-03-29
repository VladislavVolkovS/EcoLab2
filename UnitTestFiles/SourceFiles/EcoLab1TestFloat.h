//
// Created by Vladislav Volkov on 29.02.2024.
//

#ifndef ECO_LAB1_ECOLAB1TESTFLOAT_H
#define ECO_LAB1_ECOLAB1TESTFLOAT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <IEcoMemoryAllocator1.h>
#include "IEcoLab1.h"

void *CreateRootsFloatArray(IEcoMemoryAllocator1* pIMem, size_t n_roots) {
    float *out = (float*) pIMem->pVTbl->Alloc(pIMem, n_roots * sizeof(float));
	size_t i;
    srand(time(0));
    for (i = 0; i < n_roots; ++i) {
        float range = 20.0f;
        out[i] = ((float)rand() / (float)RAND_MAX) * range - 10.0f;
    }
    return out;
}

void DeleteFloatArray(IEcoMemoryAllocator1* pIMem, void* array_ptr) {
    pIMem->pVTbl->Free(pIMem, array_ptr);
}

void PrintFloatArray(void* array, size_t n_elems) {
    float* result = (float*) array;
	size_t i;
    for (i = 0; i < n_elems; ++i) {
        printf("%f ", *(result + i));
    }
    printf("\n");
}

int InitFloatOnes(void* elem) {
    *((float*)elem) = 1;
    return 0;
}

int MultFloat(const void* first, const void* second, void* result) {
    *((float*)result) =  (*((float*)first)) * (*((float*)second));
    return 0;
}

int SubFloat(const void* first, const void* second, void* result) {
    *((float*)result) =  (*((float*)first)) - (*((float*)second));
    return 0;
}

void PrintFloatTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
	void* roots_ptr;
	void* coefs;
    printf("\n\nTestFloat\n");
    roots_ptr = CreateRootsFloatArray(pIMem, n_roots);
    printf("Roots:\n");
    PrintFloatArray(roots_ptr, n_roots);
    coefs = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(float));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(float), MultFloat, SubFloat, InitFloatOnes, coefs);
    printf("Coefficients:\n");
    PrintFloatArray(coefs, n_roots + 1);
    DeleteFloatArray(pIMem, roots_ptr);
}

void WriteToFileFloatTestResults(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
	errno_t code;
	void* roots_ptr;
	void* coefs_ptr;
	float* coefficients;
	float* roots;
	size_t i;
    sprintf(csv_name, "FloatResults_%d.csv", n_roots);
    code = fopen_s(&file, csv_name, "w");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    roots_ptr = CreateRootsFloatArray(pIMem, n_roots);
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(float));
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(float), MultFloat, SubFloat, InitFloatOnes, coefs_ptr);
    fprintf(file, "%s,%s\n", "Roots", "Coefficients");
    coefficients = (float*) coefs_ptr;
    roots = (float*) roots_ptr;
    for (i = 0; i < n_roots; ++i) {
        if (i == 0) {
            fprintf(file, "%f,%f\n", roots[i], coefficients[i]);
            continue;
        }
        fprintf(file, "%f,%f\n", roots[i], coefficients[i]);
    }
    fprintf(file, " ,%f\n", coefficients[n_roots]);
    fclose(file);
    DeleteFloatArray(pIMem, roots_ptr);
}

void WriteTimeToFileFloat(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_roots) {
    FILE *file;
    char csv_name[25];
	errno_t code;
	void* roots_ptr;
	void* coefs_ptr;
	clock_t begin, end;
	double work_time;
    sprintf(csv_name, "TimeFloat.csv");
    code = fopen_s(&file, csv_name, "a");
    if (code != 0) {
        printf("File wasn't opened!");
        return;
    }
    roots_ptr = CreateRootsFloatArray(pIMem, n_roots);
    coefs_ptr = pIMem->pVTbl->Alloc(pIMem, (n_roots + 1) * sizeof(float));
    begin = clock();
    lab1->pVTbl->poly(lab1, roots_ptr, n_roots, sizeof(float), MultFloat, SubFloat, InitFloatOnes, coefs_ptr);
    end = clock();
    work_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    fprintf(file, "%d,%f\n", n_roots, work_time);
    fclose(file);
    DeleteFloatArray(pIMem, roots_ptr);
}

#endif //ECO_LAB1_ECOLAB1TESTFLOAT_H
