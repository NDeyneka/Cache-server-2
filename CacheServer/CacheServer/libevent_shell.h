#pragma once
#include "headers.h"
#include "server_request_handler.h"

class libevent_shell
{
	static const int DEFAULT_PORT = 9876;
	static void accept_error_cb(struct evconnlistener *listener, void *ctx);
	static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);
	static void echo_event_cb(struct bufferevent *bev, short events, void *ctx);
	static void echo_read_cb(struct bufferevent *bev, void *ctx);

public:
	libevent_shell();
	~libevent_shell();

	static void run_server(int port = DEFAULT_PORT);
};

