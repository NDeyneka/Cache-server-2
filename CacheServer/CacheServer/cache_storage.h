#pragma once
#include "headers.h"
#include "common_functions.h"

class cache_storage
{
	// Hash size
	static const int MODULO = 239017;

	// Base for polynomial hashing
	static const int P = 997;

	// Cache table

	// Active hashes
	static char hash_used[MODULO];

	// Stored values
	static char *cached_value[MODULO];

	// Time to live
	static int time_to_live[MODULO];

	// Get hash = (str[n - 1] + str[n - 2] * P + str[n - 3] * P^2 + ... + str[0] * P^(n-1)) % MODULO
	static int get_hash(char * str);

	// Delete value by hash
	static void delete_item(int hash);

	// Get current time for processing TTL
	static time_t get_current_time();

public:
	cache_storage();
	~cache_storage();

	// Initialize cached values and auxiliary data
	static void init_hashmap();

	// Delete all cached values
	static void clear_hashmap();

	// Get value by key
	static void get_value(char * key, char **value, int *result);

	// Set value for key
	static void set_value(char * key, char *value, int ttl);
};

