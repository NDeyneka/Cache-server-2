#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CacheServer/cache_storage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CacheServerUnitTest
{
	TEST_CLASS(cache_storage_tests)
	{
		static char * tmp;

	public:

		TEST_CLASS_INITIALIZE(class_initialize)
		{
			tmp = nullptr;
		}

		TEST_CLASS_CLEANUP(class_finalize)
		{
			if (tmp)
			{
				free(tmp);
			}
		}

		TEST_METHOD(cache_storage_test_01_init_hashmap)
		{
			// Just call for checking whether an exception occures
			cache_storage::init_hashmap();
		}

		TEST_METHOD(cache_storage_test_02_get_value)
		{
			// Get value by non-existing key
			int get_result = 0;
			cache_storage::get_value("Non-existing key", &tmp, &get_result);
			Assert::AreEqual(get_result, 0);
		}

		TEST_METHOD(cache_storage_test_03_set_value)
		{
			// Set simple key and value
			cache_storage::set_value("Key 1", "Value 1", 100);
		}

		TEST_METHOD(cache_storage_test_04_get_value)
		{
			// Get value by existing key
			int get_result = 0;
			cache_storage::get_value("Key 1", &tmp, &get_result);
			Assert::IsTrue(get_result == 1 && strcmp(tmp, "Value 1") == 0);
		}

		TEST_METHOD(cache_storage_test_05_set_value)
		{
			// Set empty key, value and ttl
			cache_storage::set_value("", "", 0);
		}

		TEST_METHOD(cache_storage_test_06_set_value)
		{
			// Set with negative ttl
			cache_storage::set_value("Key for past", "Value for past", -34);
		}

		TEST_METHOD(cache_storage_test_07_get_value)
		{
			// Get value with passed ttl
			int get_result = 0;
			cache_storage::get_value("Key for past", &tmp, &get_result);
			Assert::IsTrue(get_result == 0);
		}
		
		TEST_METHOD(cache_storage_test_08_clear_hashmap)
		{
			// Just call for checking whether an exception occures
			cache_storage::clear_hashmap();
		}
	};

	char * cache_storage_tests::tmp;
}