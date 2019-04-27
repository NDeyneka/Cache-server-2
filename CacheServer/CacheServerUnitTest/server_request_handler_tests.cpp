#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CacheServer/cache_storage.h"
#include "../CacheServer/server_request_handler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CacheServerUnitTest
{
	TEST_CLASS(server_request_handler_tests)
	{
		static char * tmp;

	public:

		TEST_CLASS_INITIALIZE(class_initialize)
		{
			tmp = nullptr;
			cache_storage::init_hashmap();
		}

		TEST_CLASS_CLEANUP(class_finalize)
		{
			if (tmp)
			{
				free(tmp);
			}
			cache_storage::clear_hashmap();
		}

		TEST_METHOD(server_request_handler_test_invalid)
		{
			// Invalid request type
			server_request_handler::process_request("INVALID REQUEST", &tmp);
			Assert::AreEqual(tmp, "ERROR|Incorrect request type (must be GET or PUT).\n");

			// Invalid delimeter
			server_request_handler::process_request("GET KEY error", &tmp);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter name and value delimeter.\n");

			// Invalid param name
			server_request_handler::process_request("GET VALUE=5", &tmp);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter names or ordering.\n");

			// Invalid params order
			server_request_handler::process_request("PUT KEY=456 TTL=4 VALUE=6", &tmp);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter names or ordering.\n");

			// Extra params in GET request (required only in PUT)
			server_request_handler::process_request("GET KEY=456 VALUE=6 TTL=4", &tmp);
			Assert::AreEqual(tmp, "ERROR|Extra symbols in request.\n");

			// Obsolete params in PUT request
			server_request_handler::process_request("PUT KEY=456", &tmp);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter names or ordering.\n");
		}

		TEST_METHOD(server_request_handler_test_get_set)
		{
			// Get non-existing key
			server_request_handler::process_request("GET KEY=123456789", &tmp);
			Assert::AreEqual(tmp, "GET FAILURE|Value does not exist.\n");

			// Set key
			server_request_handler::process_request("PUT KEY=123456789 VALUE=54345678976550 TTL=500", &tmp);
			Assert::AreEqual(tmp, "PUT SUCCESS|Value successfully saved.\n");

			// Get existing key
			server_request_handler::process_request("GET KEY=123456789", &tmp);
			Assert::AreEqual(tmp, "GET SUCCESS|54345678976550\n");

			// Set key
			server_request_handler::process_request("PUT KEY=2 VALUE=333 TTL=5", &tmp);
			Assert::AreEqual(tmp, "PUT SUCCESS|Value successfully saved.\n");

			// Get existing key
			server_request_handler::process_request("GET KEY=2", &tmp);
			Assert::AreEqual(tmp, "GET SUCCESS|333\n");

			std::this_thread::sleep_for(std::chrono::seconds(4));

			// Get existing key
			server_request_handler::process_request("GET KEY=2", &tmp);
			Assert::AreEqual(tmp, "GET SUCCESS|333\n");

			std::this_thread::sleep_for(std::chrono::seconds(2));

			// Get existing key
			server_request_handler::process_request("GET KEY=2", &tmp);
			Assert::AreEqual(tmp, "GET FAILURE|Value does not exist.\n");
		}
	};

	char * server_request_handler_tests::tmp;
}