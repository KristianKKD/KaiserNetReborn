#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using std::string;
using std::to_string;
using std::vector;
using std::thread;

int Error(string msg, int code, bool critical);
void Log(string msg);

#define LEARNINGRATE 0.01
#define MAXSIZE 4
