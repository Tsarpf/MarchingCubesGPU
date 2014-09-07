#pragma once

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
//#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

class Utilities
{
public:
	Utilities();
	~Utilities();
	void StartClock();
	double GetClock();
	void PrintToOutput(std::string);
	void PrintFrameAvg(double milliseconds);
	//void PrintToVSOutput(const char* )

private:
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	std::vector<double> results;
};

