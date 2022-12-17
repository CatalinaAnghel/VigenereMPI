#pragma once
#include "common.h"
#include "mpi.h"
#include "abstract_timer.h"
//#include <chrono>
//using namespace chrono;

class MPITimer: public AbstractTimer{
private:
	/*
	* Starting point of the time measurement
	*/
	//time_point<high_resolution_clock> startTimepoint;
	double startTimepoint;

	/*
	* Ending point of the time measurement
	*/
	//time_point<high_resolution_clock> endTimepoint;
	double endTimepoint;

	/*
	* The execution time in seconds
	*/
	double duration;
public:
	/*
	* Constructor for the Timer class
	*/
	MPITimer();

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

