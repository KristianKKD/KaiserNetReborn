#include "Shared.h"

extern bool shuttingDown;
bool shutdownInProgress;

int Error(string msg, int code, bool critical) {
	Log(msg + "(CODE " + to_string(code) + ")");

	if ((critical || shuttingDown)) { //start shutdown procedure, don't if it's already in progress
		Log("Shutting down!");
		shuttingDown = true; //some loops need this condition to be true to exit so we make sure that it's always enabled
		Log("Waiting for input to close...");
		string input;
		do { //do nothing until i manually stop it, for testing
			std::cin >> input;
		} while (input != "a");
	}

	return code;
}

void Log(double val) {
	std::cout << to_string(val) << "\n";
}

void Log(string msg) {
	if (msg.empty())
		return;

	std::cout << msg << "\n";
}