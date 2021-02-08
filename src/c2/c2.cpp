#pragma once
#include "c2.h"
// Not sure if this is needed but seems good practice?
C2Action C2Message::ActionFromID() {

	switch(action) {

		case(ACTION_ID_ERROR):
			// idk FIXME
			return C2Action();
		case(Heartbeat):
			return C2Action(this->action);

	}

	return C2Action();
}
