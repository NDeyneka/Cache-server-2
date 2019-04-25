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

		TEST_METHOD(cache_storage_test_all)
		{
			int get_result;

			// Just call for checking whether an exception occures
			cache_storage::init_hashmap();

			// Get value by non-existing key
			get_result = 0;
			cache_storage::get_value("Non-existing key", &tmp, &get_result);
			Assert::AreEqual(get_result, 0);

			// Set simple key and value
			cache_storage::set_value("Key 1", "Value 1", 100);

			// Get value by existing key
			get_result = 0;
			cache_storage::get_value("Key 1", &tmp, &get_result);
			Assert::IsTrue(get_result == 1 && strcmp(tmp, "Value 1") == 0);

			// Set empty key, value and ttl
			cache_storage::set_value("", "", 0);

			// Set with negative ttl
			cache_storage::set_value("Key for past", "Value for past", -34);

			// Get value with passed ttl
			get_result = 0;
			cache_storage::get_value("Key for past", &tmp, &get_result);
			Assert::IsTrue(get_result == 0);

			// Just call for checking whether an exception occures
			cache_storage::clear_hashmap();
		}
	};

	char * cache_storage_tests::tmp;
}