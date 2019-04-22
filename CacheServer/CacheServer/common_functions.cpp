#include "common_functions.h"



common_functions::common_functions()
{
}


common_functions::~common_functions()
{
}


void common_functions::alloc_and_copy(char ** dest, const char * source)
{
	// Delete old value if necessary
	if (*dest != NULL) {
		free(*dest);
	}
	// Alloc memory
	*dest = (char*)malloc((strlen(source) + 1) * sizeof(char));
	// Copy to dest
	strcpy_s(*dest, (strlen(source) + 1) * sizeof(char), source);
}


void common_functions::alloc_and_concat(char ** dest, const char * source1, const char * delimeter, const char * source2, int add_endline)
{
	// Delete old value if necessary
	if (*dest != NULL) {
		free(*dest);
	}
	// Alloc memory
	*dest = (char*)malloc((strlen(source1) + strlen(delimeter) + strlen(source2) + add_endline + 1) * sizeof(char));
	// Copy to dest
	int index = 0;
	for (int i = 0; source1[i]; i++) {
		(*dest)[index++] = source1[i];
	}
	for (int i = 0; delimeter[i]; i++) {
		(*dest)[index++] = delimeter[i];
	}
	for (int i = 0; source2[i]; i++) {
		(*dest)[index++] = source2[i];
	}
	if (add_endline) {
		(*dest)[index++] = '\n';
	}
	(*dest)[index] = 0;
}



