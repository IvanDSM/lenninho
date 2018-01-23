#include "global.h"
#include <math.h>
#include <stdio.h>

int ReportError(int function_result, char* error_string)
{
	int i;
	if (function_result == 1)
	{
		printf("%s%s\n", LENERR, error_string);
		return function_result - 2;
	}
	if (function_result < 0)
	{
		for (i = 0; i < function_result; i++)
		{
			printf(" ");
		}
		printf("↳%s%s\n", LENERR, error_string);
		return function_result - 1;
	}
}
