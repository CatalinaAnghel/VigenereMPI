#pragma once
#include "common.h"
#include "abstract_timer.h"
#include <chrono>
using namespace chrono;

class Timer : AbstractTimer {
private:
	/*
	* Starting point of the time measurement
	*/
	time_point<high_resolution_clock> startTimepoint;

	/*
	* Ending point of the time measurement
	*/
	time_point<high_resolution_clock> endTimepoint;

	/*
	* The execution time in seconds
	*/
	double duration;
public:
	/*
	* Constructor for the Timer class
	*/
	Timer();

	/*
	* Start the execution time measurement
	*/
	void start();

	/*
	* Finish the execution time measurement
	*/
	void stop();

	/*
	* Get the execution time
	*/
	double getDuration();
};

