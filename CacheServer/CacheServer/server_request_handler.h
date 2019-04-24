#pragma once
#include "headers.h"
#include "common_functions.h"
#include "cache_storage.h"

class server_request_handler
{
	// Set error code and error description
	static void set_error(int *error_code, char **error_description, int error_code_value, const char *error_description_value);

	static void get_parameter(const char * params, int *offset, const char* expected_param_name, 
		char **param_value, int *error_code, char **error_description);

	static void parse_request(const char * request, char ** param_type, char ** param_key, char ** param_value, char ** param_ttl,
		int *param_ttl_value, int *error_code, char **error_description);

	static void finalize_request(char * param_type, char * param_key, char * param_value, char * param_ttl,
		int error_code, char * error_description, char ** response_header, char ** response_description, char ** response);

public:
	server_request_handler();
	~server_request_handler();

	static void process_request(const char * request, char ** response);
};

