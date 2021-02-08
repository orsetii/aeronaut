#pragma once

#include <string>
#include <mutex>
#include <boost/beast/http.hpp>


enum ACTION_ID {

	ACTION_ID_ERROR = 0,
	Heartbeat = 1,
};

class C2Action {

public:


	// Should contain methods to execute, view, modify the action.
	std::string Execute();

	// TODO View function should probably send something to GUI and to stdout in debug mode.
	// Not sure how to implement this tbh.
	//virtual void View();

	// TODO add modify function, which should take in new values via params.
	C2Action() {};
	C2Action(ACTION_ID ac_id) : id(ac_id) {
		TimeSinceRequested = std::chrono::system_clock::now();
		done = false;
	}
		
	
	// Destructor
	~C2Action() {};



	ACTION_ID id;
	std::chrono::time_point<std::chrono::system_clock> TimeSinceRequested;


	// Not sure if this needed, saving this idea for later.
	//std::mutex mutex;



	bool done = false;

};

