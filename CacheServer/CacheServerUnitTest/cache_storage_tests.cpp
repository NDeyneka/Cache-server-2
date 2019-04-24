#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CacheServer/cache_storage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CacheServerUnitTest
{
	TEST_CLASS(cache_storage_tests)
	{
	public:

		TEST_METHOD(init_hashmap)
		{
			// Just call for checking whether an exception occures
			cache_storage::init_hashmap();
		}
	};
}