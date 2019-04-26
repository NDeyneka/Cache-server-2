#include "common_functions.h"



void common_functions::alloc_and_copy(char ** dest, const char * source) {
	// Delete old value if necessary
	if (*dest != NULL) {
		free(*dest);
	}
	// Alloc memory
	*dest = static_cast<char *>(malloc((strlen(source) + 1) * sizeof(char)));
	// Copy to dest
	strcpy_s(*dest, (strlen(source) + 1) * sizeof(char), source);
}


void common_functions::alloc_and_concat(char ** dest, const char * source1, const char * delimeter, const char * source2, int add_endline) {
	// Delete old value if necessary
	if (*dest != NULL) {
		free(*dest);
	}
	
	// Alloc memory
	*dest = static_cast<char *>(malloc((strlen(source1) + strlen(delimeter) + strlen(source2) + add_endline + 1) * sizeof(char)));
	
	// Copy to dest
	int index = 0;
	// Copy source 1
	for (int i = 0; source1[i]; i++) {
		(*dest)[index++] = source1[i];
	}
	// Copy delimeter
	for (int i = 0; delimeter[i]; i++) {
		(*dest)[index++] = delimeter[i];
	}
	// Copy source 2
	for (int i = 0; source2[i]; i++) {
		(*dest)[index++] = source2[i];
	}
	// Add endline if necessary
	if (add_endline) {
		(*dest)[index++] = '\n';
	}
	// Add end of string special symbol
	(*dest)[index] = 0;
}



