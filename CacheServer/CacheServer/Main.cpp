#include "libevent_shell.h"

int main() {
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	libevent_shell::run_server();

	return 0;
}