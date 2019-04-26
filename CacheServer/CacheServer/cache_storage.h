#pragma once
#include "headers.h"
#include "common_functions.h"

/*!
* \file
* \author Sitdikov T. (str719@mail.ru)
* \date   April, 2019
* \brief  Cache storage class based on hash table
*
* \section DESCRIPTION_CACHE_STORAGE
*
* cache_storage - contains cache storage class.
* The class may serve cache save and cache load queries.
*/

//--------------------------------------------------
/// \brief  Cache storage class based on hash table
///
/// This class implements cache storage functionality.
/// A cache is a triple (key, value, time to live) where both key and value are strings and time to live is an integer.
/// The class may serve two types of queries: save cache and get cache by key.
class cache_storage
{
	// Hash size
	static const int MODULO = 239017;

	// Base for polynomial hashing
	static const int P = 997;

	// Active hashes
	static char hash_used[MODULO];

	// Stored values
	static char *cached_value[MODULO];

	// Time to live
	static int time_to_live[MODULO];

	// Get hash = (str[n - 1] + str[n - 2] * P + str[n - 3] * P^2 + ... + str[0] * P^(n-1)) % MODULO
	static int get_hash(const char * str);

	// Delete value by hash
	static void delete_item(int hash);

	// Get current time for processing TTL
	static time_t get_current_time();

public:
	//! Initialize cached values and auxiliary data.
	/// \note This function must be called as initialization function, before any other calls to functions in this class.
	static void init_hashmap();

	//! Delete all cached values.
	static void clear_hashmap();

	//! Get value by key. 
	/// \param[in]  key  string, representing key for a query
	/// \param[out]  value  string for saving query result (if successful)
	/// \param[out]  result  operation result code (0 if failure, 1 if success)
	/*!
		If value corresponding to the given key exists,
	    and its time to live is not expired, returns the value and reports success.
	    Otherwise reports failure.
	*/
	static void get_value(const char * key, char **value, int *result);

	//! Set value and time to live (ttl) for key. 
	/// \param[in]  key  string, representing key for query
	/// \param[in]  value  string, representing value for a query
	/// \param[in]  ttl  time to live in seconds
	/*!
		If value already exists then overwrites both value and ttl.
	*/
	static void set_value(const char * key, const char *value, int ttl);
};

