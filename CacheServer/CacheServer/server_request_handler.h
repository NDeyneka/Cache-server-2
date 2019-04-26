#pragma once
#include "headers.h"
#include "common_functions.h"
#include "cache_storage.h"

/*!
* \file
* \author Sitdikov T. (str719@mail.ru)
* \date   April, 2019
* \brief  Request handling class
*
* \section DESCRIPTION_SERVER_REQUEST_HANDLER
*
* server_request_handler - contains cache server requests handling class.
* The class may serve cache save and cache load requests.
*/

//--------------------------------------------------
/// \brief  Server request handler class
///
/// This class processes all requests of cache server.
/// According to interaction protocol there are two types of requests - GET and PUT.
/// GET request receives cached data from server by key.
/// PUT request saves given cache on a server storage.
class server_request_handler
{
	// Maximal error description length
	static const int ERROR_DESCRIPTION_MAXIMAL_LENGTH = 4096;

	// Set error code and error description
	static void set_error(int *error_code, char **error_description, int error_code_value, const char *error_description_value);

	// Get parameter from string with perameters
	static void get_parameter(const char * params, int *offset, const char* expected_param_name, 
		char **param_value, int *error_code, char **error_description);

	// Parse request string
	static void parse_request(const char * request, char ** param_type, char ** param_key, char ** param_value, char ** param_ttl,
		int *param_ttl_value, int *error_code, char **error_description);

	// Finalize request (output response, free memory)
	static void finalize_request(char * param_type, char * param_key, char * param_value, char * param_ttl,
		int error_code, char * error_description, char ** response_header, char ** response_description, char ** response);

public:
	//! Handle cache server request. 
	/// \param[in]  request  string, representing request
	/// \param[out]  response  string for server response
	/*!
		According to server interaction protocol there are three types of requests: <br>
		+ GET - load cached data from server by key. Format: GET KEY=\<key\> 
			where \<key\> is any sequence of non-spacing letters.<br>
		+ PUT - save cache on a server. Format: PUT KEY=\<key\> VALUE=\<value\> TTL=\<ttl\> 
			where \<key\> is any sequence of non-spacing letters, 
			\<value\> is any sequence of non-spacing letters,
			\<ttl\> is time to live for value in seconds.<br>
		There are several types of responses: <br>
		+ ERROR|\<Error description\> - occures if given request has invalid format.
			\<Error description\> is a string containing a brief description of while error occured.<br>
		+ GET FAILURE|\<Failure description\> - occures only for GET requests, when request fails (no such value, etc.)
			\<Failure description\> is a string containing a brief description of while failure occured.<br>
		+ GET SUCCESS|\<Cached data\> - occures only for GET requests, when request succeeds.
			\<Cached data\> is a string containing cached data.<br>
		+ PUT SUCCESS|\<Description\> - occures only for PUT requests.
			\<Description\> is a string containing description of data saving from server.
	*/
	static void process_request(const char * request, char ** response);
};

