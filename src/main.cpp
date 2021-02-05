#include "gui/gui.h"
#include "c2/server.h"
#include <stdio.h>
#include <thread>
#include <future>



int main(int argc, char** argv) {



	// Server Thread
	auto ServerExit = std::async(std::launch::async, ServerRun, "0.0.0.0", 8080, ".");

	Gui gui(1280, 960);
	// Gui Thread
	// atm the GUI exiting exits the entire process. This could be changed? not sure.
	std::future<int> ret = std::async(&Gui::Run, &gui);

	int err = ret.get();
	if (err) {
		printf("Error in GUI. Exiting with status code %d", err);
		exit(err);
	} else {
		// Close all threads.
		exit(0);
	}
}
