#include "libevent_shell.h"



void libevent_shell::accept_error_cb(evconnlistener * listener, void * ctx) {
	// Standard error-callback function code for libevent based server
	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. "
		"Shutting down.\n", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);
}


void libevent_shell::accept_conn_cb(evconnlistener * listener, evutil_socket_t fd, sockaddr * address, int socklen, void * ctx) {
	// Standard accept-connection-callback function code for libevent based server
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	// Set callbacks
	bufferevent_setcb(bev, echo_read_cb, echo_write_cb, echo_event_cb, NULL);

	// Enable reading and writing
	bufferevent_enable(bev, EV_READ);
}


void libevent_shell::echo_event_cb(bufferevent * bev, short events, void * ctx) {
	// Standard event-callback function code for libevent based server
	if (events & BEV_EVENT_ERROR)	// Output info about error
		perror("Error from bufferevent");
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {	// Close session if EOF or error
		bufferevent_free(bev);
	}
}


void libevent_shell::echo_write_cb(bufferevent * bev, void * ctx) {
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) {
		bufferevent_free(bev);
	}
}


void libevent_shell::echo_read_cb(bufferevent * bev, void * ctx) {
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);

	// Cache server
	char *data = NULL;
	size_t  len = 0U;

	// Read requests line by line and handle
	data = evbuffer_readln(input, &len, EVBUFFER_EOL_ANY);
	
	char *response = NULL;
	
	// Handle request
	server_request_handler::process_request(data, &response);

	// Output response
	evbuffer_add(output, response, strlen(response));

	// Free memory
	free(data);
	free(response);

	// Enable write event
	bufferevent_enable(bev, EV_WRITE);
}


void libevent_shell::run_server(int port) {
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in sin;

	// Check port number
	if (port <= 0 || port > 65535) {
		puts("Invalid port");
		return;
	}

	// Create event base
	base = event_base_new();
	if (!base) {
		puts("Couldn't open event base");
		return;
	}

	// Initialize cache storage
	cache_storage::init_hashmap();

	// Below is standard code for running server with libevent

	/* Clear the sockaddr before using it, in case there are extra
	* platform-specific fields that can mess us up. */
	memset(&sin, 0, sizeof(sin));
	/* This is an INET address */
	sin.sin_family = AF_INET;
	/* Listen on 0.0.0.0 */
	sin.sin_addr.s_addr = htonl(0);
	/* Listen on the given port. */
	sin.sin_port = htons(port);

	listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
		LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
		(struct sockaddr*)&sin, sizeof(sin));
	if (!listener) {
		perror("Couldn't create listener");
		return;
	}
	evconnlistener_set_error_cb(listener, accept_error_cb);

	event_base_dispatch(base);
	return;
}



