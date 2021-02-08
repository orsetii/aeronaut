#pragma once

#include <string>
#include <chrono>
#include "action.h"
#include <vector>
#include <iostream>


typedef struct {

	// This should be 'Windows' for Windows and 
	// 'Linux' for Linux.
	std::string main_name;
	int Version;

	std::string LoggedInUser;
	std::chrono::minutes LoggedInTime;


} OS_Details;


class C2Host {

	// This class represents a host in our control
	// Should hold various data like ext/int IP,
	// hardware details
	// OS details
	// user logged on
	// uptime 
	// time of last contact
	// etc.
	//
	//
	//
	// Not sure what methods this class would use?
public:


	C2Host() {};
	C2Host(std::string name, OS_Details os) {
		Hostname = name;
		Operating_System = os;
	};


	std::string Hostname;
	OS_Details Operating_System;
	std::chrono::minutes Uptime;


};


class C2Message {

public:
	// Action
	// Host - Instance of C2Host class BECAUSE this will be passed to us from param parser
	// meaning if some of the C2Host class is empty, still works.
	//
	// Time of request
	// might be more to insert here, todo as and when.
	//
	//C2Message(ACTION_ID a, C2Host h) : action(a), host(h) {
	//	TimeOfRequest = std::chrono::system_clock::now();
	//}
	C2Message(ACTION_ID a, C2Host h) {
		printf("%d\n", __LINE__);
		host = h;
		action = a;

		printf("%d\n", __LINE__);
		TimeOfRequest = std::chrono::system_clock::now();
	}

	C2Message() {};
	
	C2Action ActionFromID();
	ACTION_ID action;
	C2Host host;
	std::chrono::time_point<std::chrono::system_clock> TimeOfRequest;


};


void InitHostTable();


