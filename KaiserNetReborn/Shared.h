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
void Log(double val);

#define LEARNINGRATE 0.001
#define MAXSIZE 4
