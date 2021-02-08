#pragma once

#include "../defs.h"
#include "param.h"
#include "c2.h"
#include "action.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
// Include some libraries for networking maybe?

class server {

	private:
	// Could implement some variables here idk.


	public:
		// Listen function that will go in its own thread.
		// When listen receives some data, serialize it,
		// and pass it off to Gui, maybe implemented as a queue?
		//
		// Whatever the data structure is, we should have the GUI method take 
		// in a pointer to it, and recv data.
		//
		// We then pass that to a GUI reading function, and we can append to that queue/stack
		// and the GUI should parse those structures.
		






};


int ServerRun(std::string address_str, int port_int, std::string doc_root_str);


// ...




