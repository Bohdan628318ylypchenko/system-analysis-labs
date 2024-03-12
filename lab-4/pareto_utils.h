#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define NULL_EXIT(ptr)                             \
	if (ptr == NULL)                               \
	{                                              \
		perror("Memory allocation returned NULL"); \
		exit(1);                                   \
	}


#define INVALID_SWITCH_EXIT(msg, value)              \
	fprintf(stderr, "%s; value = %d\n", msg, value); \
	exit(1);
