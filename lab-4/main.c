#include "pareto.h"
#include "pareto_utils.h"

#include <stdio.h>
#include <stdlib.h>

//#define ALTERNATIVE_COUNT 7
//#define CRITERIA_COUNT 4

#define ALTERNATIVE_COUNT 10
#define CRITERIA_COUNT 5

static inline void print_array(int size, const long double * restrict const array);

int main(void)
{
	long double ** data_2darr = (long double **)malloc(ALTERNATIVE_COUNT * sizeof(long double *));
	NULL_EXIT(data_2darr);
	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
	{
		data_2darr[i] = (long double *)malloc(CRITERIA_COUNT * sizeof(long double));
		NULL_EXIT(data_2darr[i]);
	}

	//data_2darr[0][0] = 40.0; data_2darr[0][1] = 8.0; data_2darr[0][2] = 4.0; data_2darr[0][3] = 0.7;
	//data_2darr[1][0] = 30.0; data_2darr[1][1] = 8.0; data_2darr[1][2] = 4.0; data_2darr[1][3] = 1.0;
	//data_2darr[2][0] = 40.0; data_2darr[2][1] = 6.0; data_2darr[2][2] = 5.0; data_2darr[2][3] = 0.6;
	//data_2darr[3][0] = 60.0; data_2darr[3][1] = 6.0; data_2darr[3][2] = 7.0; data_2darr[3][3] = 0.9;
	//data_2darr[4][0] = 45.0; data_2darr[4][1] = 7.0; data_2darr[4][2] = 4.0; data_2darr[4][3] = 0.3;
	//data_2darr[5][0] = 25.0; data_2darr[5][1] = 8.0; data_2darr[5][2] = 4.0; data_2darr[5][3] = 0.8;
	//data_2darr[6][0] = 55.0; data_2darr[6][1] = 6.0; data_2darr[6][2] = 5.0; data_2darr[6][3] = 0.7;

	data_2darr[0][0] = 4.0; data_2darr[0][1] = 0.95; data_2darr[0][2] = 35.0 ; data_2darr[0][3] = 150.0; data_2darr[0][4] = 0.75;
	data_2darr[1][0] = 4.0; data_2darr[1][1] = 0.10; data_2darr[1][2] = 72.0 ; data_2darr[1][3] = 300.0; data_2darr[1][4] = 0.34;
	data_2darr[2][0] = 3.0; data_2darr[2][1] = 0.79; data_2darr[2][2] = 31.0 ; data_2darr[2][3] = 190.0; data_2darr[2][4] = 0.65;
	data_2darr[3][0] = 2.0; data_2darr[3][1] = 0.32; data_2darr[3][2] = 24.0 ; data_2darr[3][3] = 224.0; data_2darr[3][4] = 0.94;
	data_2darr[4][0] = 5.0; data_2darr[4][1] = 0.45; data_2darr[4][2] = 88.0 ; data_2darr[4][3] = 120.0; data_2darr[4][4] = 0.15;
	data_2darr[5][0] = 8.0; data_2darr[5][1] = 0.97; data_2darr[5][2] = 44.0 ; data_2darr[5][3] = 250.0; data_2darr[5][4] = 0.64;
	data_2darr[6][0] = 3.0; data_2darr[6][1] = 1.00; data_2darr[6][2] = 29.0 ; data_2darr[6][3] = 150.0; data_2darr[6][4] = 0.79;
	data_2darr[7][0] = 7.0; data_2darr[7][1] = 0.25; data_2darr[7][2] = 115.0; data_2darr[7][3] = 140.0; data_2darr[7][4] = 0.40;
	data_2darr[8][0] = 4.0; data_2darr[8][1] = 0.78; data_2darr[8][2] = 40.0 ; data_2darr[8][3] = 220.0; data_2darr[8][4] = 0.88;
	data_2darr[9][0] = 6.0; data_2darr[9][1] = 0.10; data_2darr[9][2] = 72.0 ; data_2darr[9][3] = 165.0; data_2darr[9][4] = 0.34;
	data_2darr[9][0] = 6.0; data_2darr[9][1] = 0.51; data_2darr[9][2] = 100.0; data_2darr[9][3] = 100.0; data_2darr[9][4] = 0.38;

	criteria_type * criteria_type_1darr = (criteria_type *)malloc(CRITERIA_COUNT * sizeof(criteria_type));
	NULL_EXIT(criteria_type_1darr);

	//criteria_type_1darr[0] = MIN;
	//criteria_type_1darr[1] = MIN;
	//criteria_type_1darr[2] = MAX;
	//criteria_type_1darr[3] = QUALITY;

	criteria_type_1darr[0] = MIN;
	criteria_type_1darr[1] = QUALITY;
	criteria_type_1darr[2] = MIN;
	criteria_type_1darr[3] = MAX;
	criteria_type_1darr[4] = QUALITY;

	pareto_data * p_d = pareto_data_new(ALTERNATIVE_COUNT, CRITERIA_COUNT, data_2darr, criteria_type_1darr);

	long double * p = (long double *)malloc(p_d->criteria_count * sizeof(long double));
	NULL_EXIT(p)

	long double * r = (long double *)malloc(p_d->criteria_count * sizeof(long double));
	NULL_EXIT(r);

	long double * v = (long double *)malloc(p_d->criteria_count * sizeof(long double));
	NULL_EXIT(v);

	puts("\n==========> Original data");
	pareto_data_print(p_d);

	pareto_form_set(p_d);
	puts("\n==========> Pareto set");
	pareto_data_print(p_d);

	pareto_normalize(p_d);

	//p_d->data_2darr[1][2] = 0.67;
	//p_d->data_2darr[2][2] = 0.83;
	//p_d->data_2darr[3][2] = 1.0;
	//p_d->data_2darr[5][2] = 0.67;
	//p_d->data_2darr[6][2] = 0.83;

	puts("\n==========> Normalized Pareto set");
	pareto_data_print(p_d);

	pareto_p(p_d, p);
	puts("\n==========> Pareto P");
	print_array(p_d->criteria_count, p); putchar('\n');

	pareto_r(p_d, p, r);
	puts("\n==========> Pareto R");
	print_array(p_d->criteria_count, r); putchar('\n');

	long double R = pareto_R(p_d, r);
	printf("\n=========> Sum R: %Lf\n", R);

	pareto_v(p_d, r, R, v);
	puts("\n==========> Pareto v");
	print_array(p_d->criteria_count, v); putchar('\n');

	long double ** eM = (long double **)malloc(p_d->pareto_count * sizeof(long double *));
	NULL_EXIT(eM);
	for (int i = 0; i < p_d->pareto_count; i++)
	{
		eM[i] = (long double *)malloc(p_d->criteria_count * sizeof(long double));
		NULL_EXIT(eM[i]);
	}
	pareto_ematrix(p_d, v, p, eM);
	puts("\n==========> Pareto e");
	for (int i = 0; i < p_d->pareto_count; i++)
	{
		printf("pareto alternative %d e matrix = |", i);
		for (int j = 0; j < p_d->criteria_count; j++)
		{
			printf(" %.4Lf |", eM[i][j]);
		}
		putchar('\n');
	}

	long double * eA = (long double *)malloc(p_d->pareto_count * sizeof(long double));
	pareto_earr(p_d, eM, eA);

	puts("\n\n==========> RESULT");
	puts("\n==========> Pareto SUM e");
	print_array(p_d->pareto_count, eA);
	puts("\n==========>Pareto set");
	pareto_data_print(p_d);

	free(p); free(r); free(v); free(eA);
	for (int i = 0; i < p_d->pareto_count; i++)
		free(eM[i]);
	free(eM);

	for (int i = 0; i < CRITERIA_COUNT; i++)
		free(data_2darr[i]);
	free(data_2darr);

	free(criteria_type_1darr);

	p_d = pareto_data_free(p_d);

	putchar('\n');

	return 0;
}

static inline void print_array(int size, const long double * restrict const array)
{
	for (int i = 0; i < size; i++)
		printf("| %.4Lf ", array[i]);
	putchar('|');
}