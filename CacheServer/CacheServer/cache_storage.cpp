#include "cache_storage.h"



char cache_storage::hash_used[MODULO];
char* cache_storage::cached_value[MODULO];
int cache_storage::time_to_live[MODULO];


int cache_storage::get_hash(const char * str) {
	int hash = 0;
	// Calculate polynom on P modulo MODULO
	for (int i = 0; str[i]; i++) {
		hash = (hash * P + str[i]) % MODULO;
	}
	return hash;
}


void cache_storage::delete_item(int hash) {
	// Free memory and reset all auxiliary values for hash
	hash_used[hash] = 0;
	free(cached_value[hash]);
	cached_value[hash] = NULL;
	time_to_live[hash] = 0;
}


time_t cache_storage::get_current_time() {
	return time(NULL);
}


void cache_storage::init_hashmap() {
	// Init auxiliary values for all hashes
	for (int i = 0; i < MODULO; i++) {
		hash_used[i] = 0;
		cached_value[i] = NULL;
		time_to_live[i] = 0;
	}
}


void cache_storage::clear_hashmap() {
	// Clear all values
	for (int i = 0; i < MODULO; i++) {
		if (hash_used[i]) {
			delete_item(i);
		}
	}
}


void cache_storage::get_value(const char * key, char ** value, int * result) {
	// Get hash
	int hash = get_hash(key);

	// Check if value for key exists
	if (!hash_used[hash]) {
		*result = 0;
		return;
	}

	// Check if time to live suits 
	time_t current_time = get_current_time();
	if (time_to_live[hash] < current_time) {
		// Delete old value and return 0
		delete_item(hash);
		*result = 0;
		return;
	}

	// Get value
	*result = 1;
	common_functions::alloc_and_copy(value, cached_value[hash]);
}


void cache_storage::set_value(const char * key, const char * value, int ttl) {
	// Get hash
	int hash = get_hash(key);

	// Delete old value if necessary
	if (hash_used[hash]) {
		free(cached_value[hash]);
		cached_value[hash] = NULL;
	}

	// Set value
	hash_used[hash] = 1;
	common_functions::alloc_and_copy(&cached_value[hash], value);
	time_t current_time = get_current_time();
	time_to_live[hash] = static_cast<int>(current_time) + ttl;
}
