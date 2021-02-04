#include "gui.h"
#include <stdio.h>



int main(int argc, char** argv) {


	
	AeroGui gui (1280, 960);

	int err = gui.Run();
	
	if (err) {
		
		printf("Error in GUI. Exiting with status code %d", err);
		return err;
	}



	return 0;
}
