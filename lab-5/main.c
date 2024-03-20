#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ALTERNATIVE_COUNT 10
#define EXPERT_COUNT 6

#define MEM_ALLOC_FAIL_EXIT(ptr) if (ptr == NULL) exit(1)

static inline long double ** matrix_alloc(int rc, int cc);
static inline void matrix_print(int rc, int cc,
								const long double * restrict const * restrict const matrix);
static inline void matrix_free(long double ** matrix, int rc);
static inline long double * array_alloc(int size);
static inline void array_print(int dim, const long double * restrict const array);

int main(void)
{
	long double ** ce_m = matrix_alloc(ALTERNATIVE_COUNT, EXPERT_COUNT);

	ce_m[0][0] = 1.0;  ce_m[0][1] = 4.0; ce_m[0][2] = 3.0;  ce_m[0][3] = 5.0;  ce_m[0][4] = 1.0;  ce_m[0][5] = 4.0;
	ce_m[1][0] = 10.0; ce_m[1][1] = 6.0; ce_m[1][2] = 5.0;  ce_m[1][3] = 1.0;  ce_m[1][4] = 4.0;  ce_m[1][5] = 5.0;
	ce_m[2][0] = 6.0;  ce_m[2][1] = 2.0; ce_m[2][2] = 4.0;  ce_m[2][3] = 7.0;  ce_m[2][4] = 2.0;  ce_m[2][5] = 7.0;
	ce_m[3][0] = 3.0;  ce_m[3][1] = 8.0; ce_m[3][2] = 3.0;  ce_m[3][3] = 3.0;  ce_m[3][4] = 9.0;  ce_m[3][5] = 6.0;
	ce_m[4][0] = 7.0;  ce_m[4][1] = 9.0; ce_m[4][2] = 8.0;  ce_m[4][3] = 8.0;  ce_m[4][4] = 7.0;  ce_m[4][5] = 9.0;
	ce_m[5][0] = 10.0; ce_m[5][1] = 7.0; ce_m[5][2] = 8.0;  ce_m[5][3] = 2.0;  ce_m[5][4] = 3.0;  ce_m[5][5] = 5.0;
	ce_m[6][0] = 8.0;  ce_m[6][1] = 6.0; ce_m[6][2] = 10.0; ce_m[6][3] = 10.0; ce_m[6][4] = 7.0;  ce_m[6][5] = 10.0;
	ce_m[7][0] = 4.0;  ce_m[7][1] = 4.0; ce_m[7][2] = 4.0;  ce_m[7][3] = 9.0;  ce_m[7][4] = 5.0;  ce_m[7][5] = 7.0;
	ce_m[8][0] = 8.0;  ce_m[8][1] = 8.0; ce_m[8][2] = 10.0; ce_m[8][3] = 4.0;  ce_m[8][4] = 10.0; ce_m[8][5] = 8.0;
	ce_m[9][0] = 1.0;  ce_m[9][1] = 6.0; ce_m[9][2] = 5.0;  ce_m[9][3] = 6.0;  ce_m[9][4] = 6.0;  ce_m[9][5] = 6.0;

	puts(">>> Alternative grades by experts. Row = alternative, Column = expert:");
	matrix_print(ALTERNATIVE_COUNT, EXPERT_COUNT, ce_m);

	long double * c_sum_alternative_arr = array_alloc(ALTERNATIVE_COUNT);
	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
	{
		c_sum_alternative_arr[i] = 0;
		for (int j = 0; j < EXPERT_COUNT; j++)
			c_sum_alternative_arr[i] += ce_m[i][j];
	}
	puts(">>> Sum score for each alternative:");
	array_print(ALTERNATIVE_COUNT, c_sum_alternative_arr);

	long double c_total_sum = 0;
	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
		c_total_sum += c_sum_alternative_arr[i];
	printf(">>> c total sum = %Lf\n\n", c_total_sum);

	long double * c_weight_arr = array_alloc(ALTERNATIVE_COUNT);
	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
		c_weight_arr[i] = c_sum_alternative_arr[i] / c_total_sum;
	puts(">>> alternative weights:");
	array_print(ALTERNATIVE_COUNT, c_weight_arr);

	long double * c_average_arr = array_alloc(ALTERNATIVE_COUNT);
	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
		c_average_arr[i] = c_sum_alternative_arr[i] / (long double)EXPERT_COUNT;
	puts(">>> average scores:");
	array_print(ALTERNATIVE_COUNT, c_average_arr);

	long double * e_dispersion_arr = array_alloc(EXPERT_COUNT);
	for (int i = 0; i < EXPERT_COUNT; i++)
	{
		e_dispersion_arr[i] = 0;
		for (int j = 0; j < ALTERNATIVE_COUNT; j++)
			e_dispersion_arr[i] += powl(ce_m[j][i] - c_average_arr[j], 2.0);
		e_dispersion_arr[i] /= (ALTERNATIVE_COUNT - 1.0);
	}
	puts(">>> expert dispersion:");
	array_print(EXPERT_COUNT, e_dispersion_arr);

	long double * c_dispersion_arr = array_alloc(ALTERNATIVE_COUNT);
	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
	{
		c_dispersion_arr[i] = 0;
		for (int j = 0; j < EXPERT_COUNT; j++)
			c_dispersion_arr[i] += powl(ce_m[i][j] - c_average_arr[i], 2.0);
		c_dispersion_arr[i] /= (EXPERT_COUNT - 1.0);
	}
	puts(">>> alternative dispersion:");
	array_print(ALTERNATIVE_COUNT, c_dispersion_arr);

	matrix_free(ce_m, ALTERNATIVE_COUNT);
	free(c_sum_alternative_arr);
	free(c_weight_arr);
	free(c_average_arr);
	free(e_dispersion_arr);
	free(c_dispersion_arr);

    return 0;
}

static inline long double ** matrix_alloc(int rc, int cc)
{
	long double ** matrix = (long double **)calloc(rc, sizeof(long double *));
	MEM_ALLOC_FAIL_EXIT(matrix);
	for (int i = 0; i < rc; i++)
		matrix[i] = array_alloc(cc);
	return matrix;
}

static inline void matrix_print(int rc, int cc,
								const long double * restrict const * restrict const matrix)
{
	for (int i = 0; i < rc; i++)
		array_print(cc, matrix[i]);
}

static inline void matrix_free(long double ** matrix, int rc)
{
	for (int i = 0; i < rc; i++)
		free(matrix[i]);
	free(matrix);
}

static inline long double * array_alloc(int size)
{
	long double * array = (long double *)calloc(size, sizeof(long double));
	MEM_ALLOC_FAIL_EXIT(array);
	return array;
}

static inline void array_print(int dim, const long double * restrict const array)
{
	for (int i = 0; i < dim; i++)
		printf("| %d: %.4Lf ", i, array[i]);
	puts("|\n");
}