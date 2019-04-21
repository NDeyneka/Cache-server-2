#include "libevent_shell.h"

int main() {

	struct event_base *base;
	struct evconnlistener *listener;
	struct event *signal_event;

	struct sockaddr_in sin;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	cout << "Hi!" << endl;

	libevent_shell::test_event_init();


	return 0;
}