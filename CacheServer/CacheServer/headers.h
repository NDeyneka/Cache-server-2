#pragma once
#include <functional>
#include <iostream>

#include <mutex>
#include <thread>
#include <chrono>

#include "event.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;