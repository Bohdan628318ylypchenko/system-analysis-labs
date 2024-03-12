#include "pareto.h"
#include "pareto_utils.h"

#include <stdlib.h>
#include <stdio.h>

static inline char _pareto_set_flag_2_char(pareto_set_flag flag);
static inline char * _pareto_criteria_type_2_char(criteria_type type);

pareto_data * pareto_data_new(int alternative_count, int criteria_count,
							  long double ** data_2darr,
							  criteria_type * criteria_type_1darr)
{
	pareto_data * p_d = (pareto_data *)malloc(sizeof(pareto_data));
	NULL_EXIT(p_d);

	p_d->alternative_count = alternative_count;
	p_d->criteria_count = criteria_count;

	p_d->data_2darr = data_2darr;
	p_d->criteria_type_1darr = criteria_type_1darr;

	p_d->pareto_set = (pareto_set_flag *)malloc(alternative_count * sizeof(pareto_set_flag));
	NULL_EXIT(p_d->pareto_set);
	for (int i = 0; i < alternative_count; i++)
		p_d->pareto_set[i] = UNDERFINED;

	p_d->pareto_count = 0;
	p_d->pareto_list = (int *)malloc(alternative_count * sizeof(int));
	NULL_EXIT(p_d->pareto_list);

	return p_d;
}

pareto_data * pareto_data_free(pareto_data * p_d)
{
	free(p_d->pareto_set);
	free(p_d->pareto_list);
	free(p_d);
	return NULL;
}

void pareto_data_print(const pareto_data * restrict const p_d)
{
	printf("alternative count (row): %d; criteria count (column): %d;\n",
		   p_d->alternative_count, p_d->criteria_count);

	for (int i = 0; i < p_d->alternative_count; i++)
	{
		printf("a %d (PARETO SET FLAG: %c) = |",
			   i, _pareto_set_flag_2_char(p_d->pareto_set[i]));
		for (int j = 0; j < p_d->criteria_count; j++)
		{
			printf(" %.4Lf |", p_d->data_2darr[i][j]);
		}
		putchar('\n');
	}

	for (int i = 0; i < p_d->criteria_count; i++)
	{
		printf("criteria %d = %s\n", i, _pareto_criteria_type_2_char(p_d->criteria_type_1darr[i]));
	}

	printf("pareto count: %d;\n", p_d->pareto_count);
	if (p_d->pareto_count != 0)
	{
		for (int i = 0; i < p_d->pareto_count; i++)
		{
			printf("|%d", p_d->pareto_list[i]);
		}
		putchar('|'); putchar('\n');
	}
}

static inline char _pareto_set_flag_2_char(pareto_set_flag flag)
{
	switch (flag)
	{
	case INCLUDED:
		return 'Y';
	case EXCLUDED:
		return 'N';
	case UNDERFINED:
		return 'U';
	default:
		INVALID_SWITCH_EXIT("invalid pareto set flag", flag)
	}
}

static inline char * _pareto_criteria_type_2_char(criteria_type type)
{
	switch (type)
	{
	case MAX:
		return "max";
	case MIN:
		return "min";
	case BOOL:
		return "bool";
	case QUALITY:
		return "quality";
	}
}
