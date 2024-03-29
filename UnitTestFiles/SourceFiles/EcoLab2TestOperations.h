//
// Created by Vladislav Volkov on 29.02.2024.
//

#ifndef ECO_LAB1_ECOLAB2TESTOPERATIONS_H
#define ECO_LAB1_ECOLAB2TESTOPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IEcoLab1.h"

#define EPS 1e-6

typedef struct Data {
	int16_t x;
	int16_t y;
	int16_t add_result;
	int16_t sub_result;
	int16_t mul_result;
	int16_t div_result;
} Data, *DataPtr;

void GetRandomData(Data* data) {
	srand(time(0));
	data->x = (int)rand() % 50 - 25;
	data->y = (int)rand() % 50 - 25;
	data->add_result = data->x + data->y;
	data->sub_result = data->x - data->y;
	data->mul_result = data->x * data->y;
	data->div_result = data->x / data->y;
}

/* Возвращает 1 в случае совпадения */
int16_t CheckResult(int16_t returned, int16_t expected) {
	if (returned == expected) { 
		return 1;
	}
	return 0;
}

#endif //ECO_LAB1_ECOLAB2TESTOPERATIONS_H
