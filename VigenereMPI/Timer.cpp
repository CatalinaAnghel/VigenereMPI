#include "timer.h"

Timer::Timer() {
	this->duration = 0.0;
}

void Timer::start() {
	this->startTimepoint = high_resolution_clock::now();
}

void Timer::stop() {
	this->endTimepoint = high_resolution_clock::now();

	auto start = time_point_cast<microseconds>(this->startTimepoint).time_since_epoch().count();
	auto end = time_point_cast<microseconds>(this->endTimepoint).time_since_epoch().count();

	this->duration = end - start;
}

double Timer::getDuration() {
	return this->duration * 0.001;
}