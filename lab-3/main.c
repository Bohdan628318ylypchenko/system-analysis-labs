#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ALTERNATIVE_COUNT 3
#define CRITERIA_COUNT 5
#define VN 1.12
#define MAX_COHERENCE 0.2

#define NULL_EXIT(ptr) if (ptr == NULL) exit(1)

static inline int saati_compare(int dim, const long double * restrict const * restrict const matrix,
								long double * restrict const weight_array, long double * coherence_coefficient);
static inline void calculate_cost(int dim, const long double * restrict const * restrict const matrix,
								  long double * restrict const cost_array);
static inline long double calculate_total_cost(int dim, const long double * restrict const cost_array);
static inline void calculate_weight(int dim, long double total_cost,
									const long double * restrict const cost_array,
									long double * restrict const weight_array);
static inline void calculate_r(int dim, const long double * restrict const * restrict const matrix,
							   long double * const r_array);
static inline long double calculate_lmax(int dim,
										 const long double * restrict const r_array, const long double * restrict const weight_array);
static inline long double calculate_coherence_coefficient(int dim, long double lmax);

static inline long double ** matrix_alloc(int rc, int cc);
static inline void matrix_free(long double ** matrix, int rc);
static inline long double * array_alloc(int size);
static inline void array_print(int dim, const long double * restrict const array);

int main(void)
{
	long double ** cc_matrix = matrix_alloc(CRITERIA_COUNT, CRITERIA_COUNT);
	cc_matrix[0][0] = 1.0;       cc_matrix[0][1] = 7.0; cc_matrix[0][2] = 3.0;       cc_matrix[0][3] = 2.0;       cc_matrix[0][4] = 5.0;
	cc_matrix[1][0] = 1.0 / 7.0; cc_matrix[1][1] = 1.0; cc_matrix[1][2] = 1.0 / 7.0; cc_matrix[1][3] = 1.0 / 9.0; cc_matrix[1][4] = 1.0 / 3.0;
	cc_matrix[2][0] = 1.0 / 3.0; cc_matrix[2][1] = 7.0; cc_matrix[2][2] = 1.0;       cc_matrix[2][3] = 1.0 / 5.0; cc_matrix[2][4] = 4.0;
	cc_matrix[3][0] = 1.0 / 2.0; cc_matrix[3][1] = 9.0; cc_matrix[3][2] = 5.0;       cc_matrix[3][3] = 1.0;       cc_matrix[3][4] = 5.0;
	cc_matrix[4][0] = 1.0 / 5.0; cc_matrix[4][1] = 3.0; cc_matrix[4][2] = 1.0 / 4.0; cc_matrix[4][3] = 1.0 / 5.0; cc_matrix[4][4] = 1.0;

	long double *** cb_matrix_array = (long double ***)calloc(CRITERIA_COUNT, sizeof(long double **)); NULL_EXIT(cb_matrix_array);
	for (int i = 0; i < CRITERIA_COUNT; i++)
		cb_matrix_array[i] = matrix_alloc(ALTERNATIVE_COUNT, ALTERNATIVE_COUNT);

	cb_matrix_array[0][0][0] = 1.0;       cb_matrix_array[0][0][1] = 6.0;       cb_matrix_array[0][0][2] = 8.0;
	cb_matrix_array[0][1][0] = 1.0 / 6.0; cb_matrix_array[0][1][1] = 1.0;       cb_matrix_array[0][1][2] = 4.0;
	cb_matrix_array[0][2][0] = 1.0 / 8.0; cb_matrix_array[0][2][1] = 1.0 / 4.0; cb_matrix_array[0][2][2] = 1.0;

	cb_matrix_array[1][0][0] = 1.0;       cb_matrix_array[1][0][1] = 7.0; cb_matrix_array[1][0][2] = 1.0 / 5.0;
	cb_matrix_array[1][1][0] = 1.0 / 7.0; cb_matrix_array[1][1][1] = 1.0; cb_matrix_array[1][1][2] = 1.0 / 8.0;
	cb_matrix_array[1][2][0] = 5.0;       cb_matrix_array[1][2][1] = 8.0; cb_matrix_array[1][2][2] = 1.0;

	cb_matrix_array[2][0][0] = 1.0; cb_matrix_array[2][0][1] = 1.0 / 2.0; cb_matrix_array[2][0][2] = 1.0 / 2.0;
	cb_matrix_array[2][1][0] = 2.0; cb_matrix_array[2][1][1] = 1.0;       cb_matrix_array[2][1][2] = 1.0;
	cb_matrix_array[2][2][0] = 2.0; cb_matrix_array[2][2][1] = 1.0;       cb_matrix_array[2][2][2] = 1.0;

	cb_matrix_array[3][0][0] = 1.0; cb_matrix_array[3][0][1] = 1.0 / 7.0; cb_matrix_array[3][0][2] = 1.0 / 5.0;
	cb_matrix_array[3][1][0] = 7.0; cb_matrix_array[3][1][1] = 1.0;       cb_matrix_array[3][1][2] = 3.0;
	cb_matrix_array[3][2][0] = 5.0; cb_matrix_array[3][2][1] = 1.0 / 3.0; cb_matrix_array[3][2][2] = 1.0;

	cb_matrix_array[4][0][0] = 1.0;       cb_matrix_array[4][0][1] = 8.0; cb_matrix_array[4][0][2] = 6.0;
	cb_matrix_array[4][1][0] = 1.0 / 8.0; cb_matrix_array[4][1][1] = 1.0; cb_matrix_array[4][1][2] = 1.0 / 4.0;
	cb_matrix_array[4][2][0] = 1.0 / 6.0; cb_matrix_array[4][2][1] = 4.0; cb_matrix_array[4][2][2] = 1.0;

	long double * cc_weight_array = array_alloc(CRITERIA_COUNT);
	long double coherence_coefficient;

	puts("==========> cc");
	if (!saati_compare(CRITERIA_COUNT, cc_matrix, cc_weight_array, &coherence_coefficient))
	{
		free(cc_weight_array);
		matrix_free(cc_matrix, CRITERIA_COUNT);
		for (int i = 0; i < CRITERIA_COUNT; i++)
			matrix_free(cb_matrix_array[i], ALTERNATIVE_COUNT);
		free(cb_matrix_array);

		return 0;
	}

	long double ** cb_weight_matrix = matrix_alloc(CRITERIA_COUNT, ALTERNATIVE_COUNT);
	long double * global_priorities = array_alloc(ALTERNATIVE_COUNT);

	for (int i = 0; i < CRITERIA_COUNT; i++)
	{
		printf("\n\n==========> cb%d\n", i);
		if (!saati_compare(ALTERNATIVE_COUNT, cb_matrix_array[i], cb_weight_matrix[i], &coherence_coefficient))
		{
			free(cc_weight_array);
			matrix_free(cc_matrix, CRITERIA_COUNT);
			for (int i = 0; i < CRITERIA_COUNT; i++)
				matrix_free(cb_matrix_array[i], ALTERNATIVE_COUNT);
			free(cb_matrix_array);
			matrix_free(cb_weight_matrix, CRITERIA_COUNT);
			free(global_priorities);

			return 0;
		}
	}

	for (int i = 0; i < ALTERNATIVE_COUNT; i++)
	{
		global_priorities[i] = 0.0;
		for (int j = 0; j < CRITERIA_COUNT; j++)
		{
			global_priorities[i] += cc_weight_array[j] * cb_weight_matrix[j][i];
		}
	}
	puts("\n\n==========> global priorities: "); array_print(ALTERNATIVE_COUNT, global_priorities);

	free(cc_weight_array);
	matrix_free(cc_matrix, CRITERIA_COUNT);
	for (int i = 0; i < CRITERIA_COUNT; i++)
		matrix_free(cb_matrix_array[i], ALTERNATIVE_COUNT);
	free(cb_matrix_array);
	matrix_free(cb_weight_matrix, CRITERIA_COUNT);
	free(global_priorities);

	return 0;
}

static inline int saati_compare(int dim, const long double * restrict const * restrict const matrix,
								long double * restrict const weight_array, long double * coherence_coefficient)
{
	long double * cost_array = array_alloc(dim);
	calculate_cost(dim, matrix, cost_array);
	puts("cost array: "); array_print(dim, cost_array);

	long double total_cost = calculate_total_cost(dim, cost_array);
	printf("total cost: %.4Lf\n\n", total_cost);

	calculate_weight(dim, total_cost, cost_array, weight_array);
	puts("weight arr: "); array_print(dim, weight_array);

	long double * r_array = array_alloc(dim);
	calculate_r(dim, matrix, r_array);
	puts("r arr:"); array_print(dim, r_array);

	long double lmax = calculate_lmax(dim, r_array, weight_array);
	printf("lambda max: %.4Lf\n", lmax);

	*coherence_coefficient = calculate_coherence_coefficient(dim, lmax);
	printf("coherence coefficient: %.4Lf, max coherence: %.4Lf", *coherence_coefficient, MAX_COHERENCE);

	free(cost_array);
	free(r_array);

	if (*coherence_coefficient > MAX_COHERENCE)
	{
		puts("coherence coefficient is too big, change alternatives matrix.\n");
		return 0;
	}
	else
	{
		return 1;
	}
}

static inline void calculate_cost(int dim, const long double * restrict const * restrict const matrix,
								  long double * restrict const cost_array)
{
	for (int i = 0; i < dim; i++)
	{
		cost_array[i] = 1.0;

		for (int j = 0; j < dim; j++)
			cost_array[i] *= matrix[i][j];

		cost_array[i] = (long double)pow(cost_array[i], 1.0 / (double)dim);
	}
}

static inline long double calculate_total_cost(int dim, const long double * restrict const cost_array)
{
	long double total_cost = 0.0;
	for (int i = 0; i < dim; i++)
		total_cost += cost_array[i];
	return total_cost;
}

static inline void calculate_weight(int dim, long double total_cost,
									const long double * restrict const cost_array,
									long double * restrict const weight_array)
{
	for (int i = 0; i < dim; i++)
		weight_array[i] = cost_array[i] / total_cost;
}

static inline void calculate_r(int dim, const long double * restrict const * restrict const matrix,
							   long double * const r_array)
{
	for (int i = 0; i < dim; i++)
	{
		r_array[i] = 0.0;
		for (int j = 0; j < dim; j++)
			r_array[i] += matrix[j][i];
	}
}

static inline long double calculate_lmax(int dim,
										 const long double * restrict const r_array, const long double * restrict const weight_array)
{
	long double lmax = 0.0;
	for (int i = 0; i < dim; i++)
		lmax += r_array[i] * weight_array[i];
	return lmax;
}

static inline long double calculate_coherence_coefficient(int dim, long double lmax)
{
	return (lmax - (long double)dim) / (VN * ((long double)dim - 1.0));
}

static inline long double ** matrix_alloc(int rc, int cc)
{
	long double ** matrix = (long double **)calloc(rc, sizeof(long double *));
	NULL_EXIT(matrix);
	for (int i = 0; i < rc; i++)
		matrix[i] = array_alloc(cc);
	return matrix;
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
	NULL_EXIT(array);
	return array;
}

static inline void array_print(int dim, const long double * restrict const array)
{
	for (int i = 0; i < dim; i++)
		printf("| %d: %.4Lf ", i, array[i]);
	puts("|\n");
}
