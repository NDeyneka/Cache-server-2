#include "libevent_shell.h"

int main() {
	// Special code for windows
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	// Run cache server
	libevent_shell::run_server();

	return 0;
}