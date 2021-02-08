

#include "action.h"

#include <boost/beast/http.hpp>
#include "../defs.h"
#include "c2.h"

namespace http = boost::beast::http;



std::string C2Action::Execute() {

	
	// We can now set fields via res->set
	// Set body via res->body() = $STRING
	

	// TODO add implementation of c2 profiles.
	done = true;
	std::string body = "ack";
	return body;

}
