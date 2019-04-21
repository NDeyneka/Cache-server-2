#include "libevent_shell.h"



libevent_shell::libevent_shell()
{
}


libevent_shell::~libevent_shell()
{
}


void libevent_shell::test_event_init()
{
	if (!event_init())
		cout << "Failed to init libevent." << endl;
	else
		cout << "libevent init successfully!" << endl;
	getchar();
}
