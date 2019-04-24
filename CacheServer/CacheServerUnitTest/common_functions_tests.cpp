#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CacheServer/common_functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CacheServerUnitTest
{		
	TEST_CLASS(common_functions_tests)
	{
	public:
		
		TEST_METHOD(alloc_and_copy)
		{
			char * tmp = nullptr;

			// Simple case - copy simple string
			common_functions::alloc_and_copy(&tmp, "First test");
			Assert::AreEqual(tmp, "First test");

			// Corner case - copy empty string
			common_functions::alloc_and_copy(&tmp, "");
			Assert::AreEqual(tmp, "");

			if (tmp)
			{
				free(tmp);
			}
		}

		TEST_METHOD(alloc_and_concat)
		{
			char *tmp = nullptr;

			// Simple case - concat two simple strings and add endline
			common_functions::alloc_and_concat(&tmp, "First string", " & ", "Second string", 1);
			Assert::AreEqual(tmp, "First string & Second string\n");

			// Simple case - concat two simple strings and do not add endline
			common_functions::alloc_and_concat(&tmp, "Prefix", "|", "Suffix", 0);
			Assert::AreEqual(tmp, "Prefix|Suffix");

			// Corner case - three empty strings
			common_functions::alloc_and_concat(&tmp, "", "", "", 0);
			Assert::AreEqual(tmp, "");

			// Corner case - three empty strings and endline
			common_functions::alloc_and_concat(&tmp, "", "", "", 1);
			Assert::AreEqual(tmp, "\n");

			// Corner case - empty delimeter
			common_functions::alloc_and_concat(&tmp, "Hello", "", "World", 1);
			Assert::AreEqual(tmp, "HelloWorld\n");

			// Corner case - everything empty except delimeter
			common_functions::alloc_and_concat(&tmp, "", "HugeDelimeter", "", 0);
			Assert::AreEqual(tmp, "HugeDelimeter");

			if (tmp)
			{
				free(tmp);
			}
		}
	};
}