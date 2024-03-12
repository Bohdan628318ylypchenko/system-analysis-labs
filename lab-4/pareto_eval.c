#include "pareto.h"
#include "pareto_utils.h"

#include <float.h>
#include <math.h>
#include <stdio.h>

static inline int _is_1st_better_by_all_criteria_2nd(int criteria_count,
										             const criteria_type * restrict const criteria_type_1darr,
										             const long double * restrict const a1,
										             const long double * restrict const a2);
static inline void _normalize_bool(int criteria_index, pareto_data * restrict const p_d);
static inline void _normalize_max(int criteria_index, pareto_data * restrict const p_d);
static inline void _normalize_min(int criteria_index, pareto_data * restrict const p_d);

void pareto_form_set(pareto_data * restrict const p_d)
{
	for (int i = 0; i < p_d->alternative_count; i++)
	{
		if (p_d->pareto_set[i] == EXCLUDED)
			continue;

		for (int j = 0; j < p_d->alternative_count; j++)
		{
			if ((i == j) || (p_d->pareto_set[j] == EXCLUDED))
				continue;

			if (_is_1st_better_by_all_criteria_2nd(p_d->criteria_count, p_d->criteria_type_1darr,
										           p_d->data_2darr[i], p_d->data_2darr[j]))
				p_d->pareto_set[j] = EXCLUDED;
		}
	}

	for (int i = 0; i < p_d->alternative_count; i++)
	{
		if (p_d->pareto_set[i] == EXCLUDED)
			continue;

		p_d->pareto_set[i] = INCLUDED;
		p_d->pareto_list[p_d->pareto_count++] = i;
	}
}

void pareto_normalize(pareto_data * restrict const p_d)
{
	for (int i = 0; i < p_d->criteria_count; i++)
	{
		switch (p_d->criteria_type_1darr[i])
		{
		case BOOL:
			_normalize_bool(i, p_d);
			break;
		case MAX:
			_normalize_max(i, p_d);
			break;
		case MIN:
			_normalize_min(i, p_d);
			break;
		case QUALITY:
			break;
		default:
			INVALID_SWITCH_EXIT("invalid criteria type", p_d->criteria_type_1darr[i])
		}
	}
}

void pareto_p(const pareto_data * restrict const p_d, long double * restrict const p)
{
	for (int i = 0; i < p_d->criteria_count; i++)
	{
		p[i] = 0.0;
		for (int j = 0; j < p_d->pareto_count; j++)
			p[i] += p_d->data_2darr[p_d->pareto_list[j]][i];
		p[i] /= p_d->pareto_count;
	}
}

void pareto_r(const pareto_data * restrict const p_d, const long double * restrict const p,
			  long double * restrict const r)
{
	for (int i = 0; i < p_d->criteria_count; i++)
	{
		r[i] = 0.0;
		for (int j = 0; j < p_d->pareto_count; j++)
		{
			//long double pji = p_d->data_2darr[p_d->pareto_list[j]][i];
			//printf("| %Lf - %Lf | ", pji, p[i]);
			r[i] += fabsl(p_d->data_2darr[p_d->pareto_list[j]][i] - p[i]);
		}
			//r[i] += (p_d->data_2darr[p_d->pareto_list[j]][i] - p[i]) < 0 ?
			//-1.0 * (p_d->data_2darr[p_d->pareto_list[j]][i] - p[i]) : (p_d->data_2darr[p_d->pareto_list[j]][i] - p[i]);
		r[i] /= ((long double)p_d->pareto_count * p[i]);
		//putchar('\n');
	}
}

long double pareto_R(const pareto_data * restrict const p_d,
					 const long double * restrict const r)
{
	long double R = 0.0;
	for (int i = 0; i < p_d->criteria_count; i++)
		R += r[i];
	return R;
}

void pareto_v(const pareto_data * restrict const p_d,
			  const long double * restrict const r, long double R,
			  long double * restrict const v)
{
	for (int i = 0; i < p_d->criteria_count; i++)
		v[i] = r[i] / R;
}

void pareto_ematrix(const pareto_data * restrict const p_d,
					const long double * restrict const v, const long double * restrict const p,
					long double * restrict const * restrict const eM)
{
	for (int i = 0; i < p_d->pareto_count; i++)
		for (int j = 0; j < p_d->criteria_count; j++)
			eM[i][j] = v[j] / p_d->data_2darr[p_d->pareto_list[i]][j];
}

void pareto_earr(const pareto_data * restrict const p_d,
				 const long double * restrict const * restrict const eM,
				 long double * restrict const eA)
{
	for (int i = 0; i < p_d->pareto_count; i++)
	{
		eA[i] = 0.0;
		for (int j = 0; j < p_d->criteria_count; j++)
			eA[i] += eM[i][j];
	}
}

static inline int _is_1st_better_by_all_criteria_2nd(int criteria_count,
										             const criteria_type * restrict const criteria_type_1darr,
										             const long double * restrict const a1,
										             const long double * restrict const a2)
{
	int score = 0;

	for (int i = 0; i < criteria_count; i++)
	{
		switch (criteria_type_1darr[i])
		{
		case BOOL:
		case QUALITY:
		case MAX:
			if (a1[i] >= a2[i])
				score++;
			else
				score--;
			break;
		case MIN:
			if (a1[i] <= a2[i])
				score++;
			else
				score--;
			break;
		default:
			INVALID_SWITCH_EXIT("invalid criteria type", criteria_type_1darr[i])
		}
	}

	return score == criteria_count;
}

static inline void _normalize_bool(int criteria_index, pareto_data * restrict const p_d)
{
	for (int i = 0; i < p_d->pareto_count; i++)
	{
		if (p_d->data_2darr[p_d->pareto_list[i]][criteria_index])
			p_d->data_2darr[p_d->pareto_list[i]][criteria_index] = NUMERIC_TRUE;
		else
			p_d->data_2darr[p_d->pareto_list[i]][criteria_index] = NUMERIC_FALSE;
	}
}

static inline void _normalize_max(int criteria_index, pareto_data * restrict const p_d)
{
	long double max = LDBL_MIN;

	for (int i = 0; i < p_d->pareto_count; i++)
		if (max < p_d->data_2darr[p_d->pareto_list[i]][criteria_index])
			max = p_d->data_2darr[p_d->pareto_list[i]][criteria_index];

	if (max == 0.0)
		return;

	for (int i = 0; i < p_d->pareto_count; i++)
		p_d->data_2darr[p_d->pareto_list[i]][criteria_index] /= max;
}

static inline void _normalize_min(int criteria_index, pareto_data * restrict const p_d)
{
	long double min = LDBL_MAX;

	for (int i = 0; i < p_d->pareto_count; i++)
		if (min > p_d->data_2darr[p_d->pareto_list[i]][criteria_index])
			min = p_d->data_2darr[p_d->pareto_list[i]][criteria_index];

	for (int i = 0; i < p_d->pareto_count; i++)
		if (p_d->data_2darr[p_d->pareto_list[i]][criteria_index] != 0)
			p_d->data_2darr[p_d->pareto_list[i]][criteria_index] =
			min / p_d->data_2darr[p_d->pareto_list[i]][criteria_index];
}
