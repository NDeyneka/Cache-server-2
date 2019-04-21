#include <iostream>
#include "event.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

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

	if (!event_init())
		cout << "Failed to init libevent." << endl;
	else
		cout << "libevent init successfully!" << endl;


	return 0;
}