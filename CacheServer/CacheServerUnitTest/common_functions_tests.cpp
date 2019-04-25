#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CacheServer/common_functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CacheServerUnitTest
{	
	TEST_CLASS(common_functions_tests)
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
		
		TEST_METHOD(alloc_and_copy_1)
		{
			// Simple case - copy simple string
			common_functions::alloc_and_copy(&tmp, "First test");
			Assert::AreEqual(tmp, "First test");
		}

		TEST_METHOD(alloc_and_copy_2)
		{
			// Corner case - copy empty string
			common_functions::alloc_and_copy(&tmp, "");
			Assert::AreEqual(tmp, "");
		}

		TEST_METHOD(alloc_and_concat_1)
		{
			// Simple case - concat two simple strings and add endline
			common_functions::alloc_and_concat(&tmp, "First string", " & ", "Second string", 1);
			Assert::AreEqual(tmp, "First string & Second string\n");
		}

		TEST_METHOD(alloc_and_concat_2)
		{
			// Simple case - concat two simple strings and do not add endline
			common_functions::alloc_and_concat(&tmp, "Prefix", "|", "Suffix", 0);
			Assert::AreEqual(tmp, "Prefix|Suffix");
		}

		TEST_METHOD(alloc_and_concat_3)
		{
			// Corner case - three empty strings
			common_functions::alloc_and_concat(&tmp, "", "", "", 0);
			Assert::AreEqual(tmp, "");
		}

		TEST_METHOD(alloc_and_concat_4)
		{
			// Corner case - three empty strings and endline
			common_functions::alloc_and_concat(&tmp, "", "", "", 1);
			Assert::AreEqual(tmp, "\n");
		}

		TEST_METHOD(alloc_and_concat_5)
		{
			// Corner case - empty delimeter
			common_functions::alloc_and_concat(&tmp, "Hello", "", "World", 1);
			Assert::AreEqual(tmp, "HelloWorld\n");
		}

		TEST_METHOD(alloc_and_concat_6)
		{
			// Corner case - everything empty except delimeter
			common_functions::alloc_and_concat(&tmp, "", "HugeDelimeter", "", 0);
			Assert::AreEqual(tmp, "HugeDelimeter");
		}
	};

	char * common_functions_tests::tmp;
}