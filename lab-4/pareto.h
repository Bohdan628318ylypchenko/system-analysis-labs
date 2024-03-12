#pragma once

#define NUMERIC_TRUE 0.67
#define NUMERIC_FALSE 0.33

typedef enum { MAX, MIN, BOOL, QUALITY } criteria_type;

typedef enum { INCLUDED, EXCLUDED, UNDERFINED } pareto_set_flag;

typedef struct
{
	int alternative_count;
	int criteria_count;

	long double ** data_2darr;
	criteria_type * criteria_type_1darr;

	pareto_set_flag * pareto_set;

	int pareto_count;
	int * pareto_list;

} pareto_data;

pareto_data * pareto_data_new(int alternative_count, int criteria_count,
						   	  long double ** data_2darr,
							  criteria_type * criteria_type_1darr);
pareto_data * pareto_data_free(pareto_data * p_d);
void pareto_data_print(const pareto_data * restrict const p_d);

void pareto_form_set(pareto_data * restrict const p_d);
void pareto_normalize(pareto_data * restrict const p_d);

void pareto_p(const pareto_data * restrict const p_d, long double * restrict const p);
void pareto_r(const pareto_data * restrict const p_d, const long double * restrict const p,
			  long double * restrict const r);

long double pareto_R(const pareto_data * restrict const p_d,
					 const long double * restrict const r);

void pareto_v(const pareto_data * restrict const p_d,
			  const long double * restrict const r, long double R,
			  long double * restrict const v);

void pareto_ematrix(const pareto_data * restrict const p_d,
					const long double * restrict const v, const long double * restrict const p,
					long double * restrict const * restrict const eM);

void pareto_earr(const pareto_data * restrict const p_d,
				 const long double * restrict const * restrict const eM,
				 long double * restrict const eA);
