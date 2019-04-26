#include "server_request_handler.h"



void server_request_handler::set_error(int * error_code, char ** error_description, int error_code_value, const char * error_description_value) {
	*error_code = error_code_value;
	strcpy_s(*error_description, ERROR_DESCRIPTION_MAXIMAL_LENGTH, error_description_value);
}


void server_request_handler::get_parameter(const char * params, int * offset, const char * expected_param_name, char ** param_value, 
	int * error_code, char ** error_description) {
	// If already error - do nothing
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
		// Delimeter of param name and value must be '='
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


void server_request_handler::parse_request(const char * request, char ** param_type, char ** param_key, char ** param_value, 
	char ** param_ttl, int * param_ttl_value, int * error_code, char ** error_description) {
	int offset = 0;

	// Get type of request (GET | PUT | EXIT)
	get_parameter(request, &offset, "", param_type, error_code, error_description);
	if (!(strcmp(*param_type, "GET") == 0 || strcmp(*param_type, "PUT") == 0 || strcmp(*param_type, "EXIT") == 0)) {
		set_error(error_code, error_description, 3, "Incorrect request type (must be GET, PUT or EXIT).");
		return;
	}

	// Get additional params for GET | PUT requests
	if (strcmp(*param_type, "GET") == 0 || strcmp(*param_type, "PUT") == 0) {
		// Key (for GET and PUT)
		get_parameter(request, &offset, "KEY", param_key, error_code, error_description);
		if (strcmp(*param_type, "PUT") == 0) {
			// Value for PUT
			get_parameter(request, &offset, "VALUE", param_value, error_code, error_description);
			// time to live for PUT
			get_parameter(request, &offset, "TTL", param_ttl, error_code, error_description);
		}
	}

	// Handle error during reading parameters
	if ((*error_code) != 0) {
		return;
	}

	// Extra symbols - error
	if (request[offset] != 0) {
		set_error(error_code, error_description, 4, "Extra symbols in request.");
		return;
	}

	// Parse and calculate time to live as integer
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


void server_request_handler::finalize_request(char * param_type, char * param_key, char * param_value, char * param_ttl, 
	int error_code, char * error_description, char ** response_header, char ** response_description, char ** response) {
	// Process special cases
	if (error_code != 0) {	// Error
		common_functions::alloc_and_copy(response_header, "ERROR");
		common_functions::alloc_and_copy(response_description, error_description);
	} else if (strcmp(param_type, "EXIT") == 0) {	// Exit
		common_functions::alloc_and_copy(response_header, "EXIT");
		common_functions::alloc_and_copy(response_description, "OK");
	}

	// Copy response
	common_functions::alloc_and_concat(response, *response_header, "|", *response_description, 1);

	// Free memory
	free(param_type);
	free(param_key);
	free(param_value);
	free(param_ttl);
	free(error_description);

	free(*response_header);
	free(*response_description);
}


void server_request_handler::process_request(const char * request, char ** response, int *session_close_flag) {
	// Set error fields
	int error_code = 0;
	char * error_description = static_cast<char *>(malloc(ERROR_DESCRIPTION_MAXIMAL_LENGTH * sizeof(char)));

	// Parse request
	char * param_type = NULL, *param_key = NULL, *param_value = NULL, *param_ttl = NULL;
	int ttl = 0;
	char * response_header = NULL, *response_description = NULL;
	parse_request(request, &param_type, &param_key, &param_value, &param_ttl, &ttl, &error_code, &error_description);

	// If error - handle error and return
	if (error_code != 0) {
		finalize_request(param_type, param_key, param_value, param_ttl,
			error_code, error_description, &response_header, &response_description, response);
		return;
	}

	// Handle special EXIT request (just for finishng session)
	*session_close_flag = 0;
	if (strcmp(param_type, "EXIT") == 0) {
		*session_close_flag = 1;
		finalize_request(param_type, param_key, param_value, param_ttl,
			error_code, error_description, &response_header, &response_description, response);
		return;
	}

	// Handle GET request
	if (strcmp(param_type, "GET") == 0) {
		int result = 0;
		cache_storage::get_value(param_key, &param_value, &result);	// Handle request
		if (result == 0) {	// If success
			common_functions::alloc_and_copy(&response_header, "GET FAILURE");
			common_functions::alloc_and_copy(&response_description, "Value does not exist.");
		} else {	// If failure
			common_functions::alloc_and_copy(&response_header, "GET SUCCESS");
			common_functions::alloc_and_copy(&response_description, param_value);
		}
	} else if (strcmp(param_type, "PUT") == 0) {	// Handle PUT request
		cache_storage::set_value(param_key, param_value, ttl);	// Handle request
		common_functions::alloc_and_copy(&response_header, "PUT SUCCESS");
		common_functions::alloc_and_copy(&response_description, "Value successfully saved.");
	}

	// Finalize request (output result, free memory)
	finalize_request(param_type, param_key, param_value, param_ttl,
		error_code, error_description, &response_header, &response_description, response);
	return;
}
