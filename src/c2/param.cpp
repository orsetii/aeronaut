#include "param.h"


static std::map<uint, C2Host> HostTable;

// TODO take the opcode order/format/chars from a c2 profile file.
static ACTION_ID ParseOpcode(char opcode) {


	switch(opcode) {
		case('h'):
			return Heartbeat;

	};
	return ACTION_ID_ERROR;
}


C2Message ParseParams(std::string param_string) {


	// Validation/checks
	// C2Host Constructor
	// to
	// Validation/Checks 
	// C2Message Constructor
	//
	// return
	HostTable.insert(std::pair<unsigned int, C2Host>(0, C2Host()));

	std::string substr = param_string.substr(0, 1);
	unsigned int host_id = 0;
	auto found_host = HostTable.find(host_id);
	
	if (found_host->first == HostTable.end()->first) {

		// If not found we add this to the table.
		// TODO implement a reply to this to request additional data
		// such as hostname, ip etc.
		std::printf("%02x Host ID is not a known Host. TODO function to add unknown to known hosts", host_id);

	} else {
		std::printf("FOUND!!!\n\n");
	}

	ACTION_ID actionid = ParseOpcode(param_string.at(2));
	printf("\nActionID: %d\n", actionid);

	auto ret = C2Message();
	ret.action = actionid;
	ret.host = found_host->second;
	return ret;



}





