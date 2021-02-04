#include "gui/gui.h"
#include <stdio.h>
#include <thread>
#include <future>



int main(int argc, char** argv) {


	
	Gui gui(1280, 960);

	// Gui Thread
	std::future<int> ret = std::async(&Gui::Run, &gui);

	// Server Thread goes here..






	int err = ret.get();
	if (err) {
		
		printf("Error in GUI. Exiting with status code %d", err);
		return err;
	}



	return 0;
}
