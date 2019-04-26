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
			int session_close_flag = 0;

			// Invalid request type
			server_request_handler::process_request("INVALID REQUEST", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "ERROR|Incorrect request type (must be GET, PUT or EXIT).\n");

			// Invalid delimeter
			server_request_handler::process_request("GET KEY error", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter name and value delimeter.\n");

			// Invalid param name
			server_request_handler::process_request("GET VALUE=5", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter names or ordering.\n");

			// Invalid params order
			server_request_handler::process_request("PUT KEY=456 TTL=4 VALUE=6", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter names or ordering.\n");

			// Extra params in GET request (required only in PUT)
			server_request_handler::process_request("GET KEY=456 VALUE=6 TTL=4", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "ERROR|Extra symbols in request.\n");

			// Obsolete params in PUT request
			server_request_handler::process_request("PUT KEY=456", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "ERROR|Incorrect parameter names or ordering.\n");
		}

		TEST_METHOD(server_request_handler_test_get_set)
		{
			int session_close_flag = 0;

			// Get non-existing key
			server_request_handler::process_request("GET KEY=123456789", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "GET FAILURE|Value does not exist.\n");

			// Set key
			server_request_handler::process_request("PUT KEY=123456789 VALUE=54345678976550 TTL=500", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "PUT SUCCESS|Value successfully saved.\n");

			// Get existing key
			server_request_handler::process_request("GET KEY=123456789", &tmp, &session_close_flag);
			Assert::AreEqual(tmp, "GET SUCCESS|54345678976550\n");
		}

		TEST_METHOD(server_request_handler_test_exit)
		{
			int session_close_flag = 0;

			// Get non-existing key
			server_request_handler::process_request("EXIT", &tmp, &session_close_flag);
			Assert::AreEqual(session_close_flag, 1);
		}
	};

	char * server_request_handler_tests::tmp;
}