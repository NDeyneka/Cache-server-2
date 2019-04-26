#pragma once
#include "headers.h"
#include "server_request_handler.h"

#include "event.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

/*!
* \file
* \author Sitdikov T. (str719@mail.ru)
* \date   April, 2019
* \brief  Server running class
*
* \section DESCRIPTION_LIBEVENT_SHELL
*
* libevent_shell - contains cache server running functions.
* Cache server is based on libevent library.
*/

//--------------------------------------------------
/// \brief  Cache server running class
///
/// This class runs cache server using libevent library.
class libevent_shell
{
	// Default port number
	static const int DEFAULT_PORT = 9876;

	// Libevent callback function for error
	static void accept_error_cb(struct evconnlistener *listener, void *ctx);

	// Libevent callback funciotn for accepting connection
	static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);

	// Libevent callback function for events
	static void echo_event_cb(struct bufferevent *bev, short events, void *ctx);

	// Libevent callback function for writing
	static void echo_write_cb(struct bufferevent *bev, void *ctx);

	// Libevent callback function for reading
	static void echo_read_cb(struct bufferevent *bev, void *ctx);

public:
	//! Run cache server. 
	/// \param[in]  port  int, server port number, 9876 by default
	static void run_server(int port = DEFAULT_PORT);
};

