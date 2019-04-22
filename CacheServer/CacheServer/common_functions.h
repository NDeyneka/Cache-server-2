#pragma once
#include "headers.h"

class common_functions
{
public:
	common_functions();
	~common_functions();

	// Alloc memory and set dest = source
	static void alloc_and_copy(char **dest, const char *source);

	// alloc memory and set dest = source1 + delimeter + source2
	static void alloc_and_concat(char **dest, const char * source1, const char * delimeter, const char *source2, int add_endline);
};

