#include "server_request_handler.h"



void server_request_handler::set_error(int * error_code, char ** error_description, int error_code_value, const char * error_description_value)
{
	*error_code = error_code_value;
	strcpy_s(*error_description, 4096, error_description_value);
}


void server_request_handler::get_parameter(const char * params, int * offset, const char * expected_param_name, char ** param_value, int * error_code, char ** error_description)
{
	// Already error - do nothing
	if ((*error_code) != 0) {
		return;
	}

	// Non-empty expected parameter name - process the name
	if (expected_param_name[0] != 0) {
		// Check parameter name	
		for (int i = 0; expected_param_name[i]; i++) {
			if (params[(*offset)] == 0 || params[(*offset)] != expected_param_name[i]) {
				// Invalid parameter name - error code 1
				set_error(error_code, error_description, 1, "Incorrect parameter names or ordering.");
				return;
			}
			(*offset)++;
		}
		// Delimeter of param name and value should be '='
		if (params[(*offset)] != '=') {
			// Invalid delimeter - error code 2
			set_error(error_code, error_description, 2, "Incorrect parameter name and value delimeter.");
			return;
		}
		(*offset)++;
	}

	// Find the length of parameter value (from offset to delimeter
	// which is symbol with code less or equal to 32 (space, EOL, EOF, \t, ...))
	int param_value_length;
	for (param_value_length = 0; params[(*offset) + param_value_length] > 32; param_value_length++);
	// Set param value
	*param_value = static_cast<char *>(malloc(param_value_length + 1));
	for (int i = 0; i < param_value_length; i++) {
		(*param_value)[i] = params[(*offset) + i];
	}
	(*param_value)[param_value_length] = 0;
	(*offset) += param_value_length;
	// Process additional symbol if necessary
	if (params[(*offset)] != 0 && params[(*offset)] != EOF) {
		(*offset)++;
	}

	return;
}


void server_request_handler::parse_request(const char * request, char ** param_type, char ** param_key, char ** param_value, char ** param_ttl, int * param_ttl_value, int * error_code, char ** error_description)
{
	int offset = 0;

	get_parameter(request, &offset, "", param_type, error_code, error_description);
	// Type of request (GET | PUT)
	if (!(strcmp(*param_type, "GET") == 0 || strcmp(*param_type, "PUT") == 0)) {
		set_error(error_code, error_description, 3, "Incorrect request type (must be GET or PUT).");
		return;
	}
	if (strcmp(*param_type, "GET") == 0 || strcmp(*param_type, "PUT") == 0) {
		// Key
		get_parameter(request, &offset, "KEY", param_key, error_code, error_description);
		if (strcmp(*param_type, "PUT") == 0) {
			// Value
			get_parameter(request, &offset, "VALUE", param_value, error_code, error_description);
			// time to live
			get_parameter(request, &offset, "TTL", param_ttl, error_code, error_description);
		}
	}
	if ((*error_code) != 0) {
		return;
	}
	if (request[offset] != 0) {
		set_error(error_code, error_description, 4, "Extra symbols in request.");
		return;
	}
	*param_ttl_value = 0;
	if (strcmp(*param_type, "PUT") == 0) {
		for (int i = 0; (*param_ttl)[i]; i++) {
			if ((*param_ttl)[i] < '0' || (*param_ttl)[i] > '9') {
				set_error(error_code, error_description, 5, "Invalid ttl format.");
				return;
			}
			*param_ttl_value = *param_ttl_value * 10 + ((*param_ttl)[i] - '0');
		}
	}
	return;
}


void server_request_handler::finalize_request(char * param_type, char * param_key, char * param_value, char * param_ttl, int error_code, char * error_description, char ** response_header, char ** response_description, char ** response)
{
	if (error_code != 0) {
		common_functions::alloc_and_copy(response_header, "ERROR");
		common_functions::alloc_and_copy(response_description, error_description);
	}

	common_functions::alloc_and_concat(response, *response_header, "|", *response_description, 1);

	free(param_type);
	free(param_key);
	free(param_value);
	free(param_ttl);
	free(error_description);

	free(*response_header);
	free(*response_description);
}


void server_request_handler::process_request(const char * request, char ** response)
{
	// Set error fields
	int error_code = 0;
	char * error_description = static_cast<char *>(malloc(4096 * sizeof(char)));

	// Parse request
	char * param_type = NULL, *param_key = NULL, *param_value = NULL, *param_ttl = NULL;
	int ttl = 0;
	char * response_header = NULL, *response_description = NULL;
	parse_request(request, &param_type, &param_key, &param_value, &param_ttl, &ttl, &error_code, &error_description);
	if (error_code != 0) {
		finalize_request(param_type, param_key, param_value, param_ttl,
			error_code, error_description, &response_header, &response_description, response);
		return;
	}

	if (strcmp(param_type, "GET") == 0) {
		int result = 0;
		cache_storage::get_value(param_key, &param_value, &result);
		if (result == 0) {
			common_functions::alloc_and_copy(&response_header, "GET FAILURE");
			common_functions::alloc_and_copy(&response_description, "Value does not exist.");
		}
		else {
			common_functions::alloc_and_copy(&response_header, "GET SUCCESS");
			common_functions::alloc_and_copy(&response_description, param_value);
		}
	}
	else if (strcmp(param_type, "PUT") == 0) {
		cache_storage::set_value(param_key, param_value, ttl);
		common_functions::alloc_and_copy(&response_header, "PUT SUCCESS");
		common_functions::alloc_and_copy(&response_description, "Value successfully saved.");
	}

	finalize_request(param_type, param_key, param_value, param_ttl,
		error_code, error_description, &response_header, &response_description, response);
	return;
}
