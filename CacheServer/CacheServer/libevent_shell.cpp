#include "libevent_shell.h"



void libevent_shell::accept_error_cb(evconnlistener * listener, void * ctx) {
	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. "
		"Shutting down.\n", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);
}


void libevent_shell::accept_conn_cb(evconnlistener * listener, evutil_socket_t fd, sockaddr * address, int socklen, void * ctx) {
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(
		base, fd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bev, echo_read_cb, NULL, echo_event_cb, NULL);

	bufferevent_enable(bev, EV_READ | EV_WRITE);
}


void libevent_shell::echo_event_cb(bufferevent * bev, short events, void * ctx) {
	if (events & BEV_EVENT_ERROR)
		perror("Error from bufferevent");
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
		bufferevent_free(bev);
	}
}


void libevent_shell::echo_read_cb(bufferevent * bev, void * ctx) {
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);

	// Echo-server
	size_t len = evbuffer_get_length(input);
	char *data = NULL;
	data = (char*)malloc(len + 1);
	data[len] = 0;
	evbuffer_copyout(input, data, len);
	printf("we got some data: %s\n", data);
	free(data);

	evbuffer_add_buffer(output, input);
}


libevent_shell::libevent_shell() {
}


libevent_shell::~libevent_shell() {
}


void libevent_shell::test_event_init() {
	if (!event_init())
		cout << "Failed to init libevent." << endl;
	else
		cout << "libevent init successfully!" << endl;
	getchar();
}


void libevent_shell::run_server(int port) {

}



