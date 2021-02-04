#include "gui/gui.h"
#include "c2/server.h"
#include <stdio.h>
#include <thread>
#include <future>



int main(int argc, char** argv) {



	// Server Thread goes here..
	// Server Init function should return a *queue, 
	// and we then pass a pointer to that to the GUI thread.
	//
	// That queue should be updated with serialized data,
	// and the GUI should process that frontend as it pleases.
	
	Gui gui(1280, 960);

	// Gui Thread
	std::future<int> ret = std::async(&Gui::Run, &gui);

	






	int err = ret.get();
	if (err) {
		
		printf("Error in GUI. Exiting with status code %d", err);
		return err;
	}



	return 0;
}
