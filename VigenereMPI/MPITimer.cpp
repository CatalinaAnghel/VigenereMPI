#include "mpi_timer.h"

MPITimer::MPITimer(){
	this->duration = 0.0;
	this->startTimepoint = 0.0;
	this->endTimepoint = 0.0;
}

void MPITimer::start() {
	this->startTimepoint = MPI_Wtime();
	//this->startTimepoint = high_resolution_clock::now();
}

void MPITimer::stop() {
	/*this->endTimepoint = high_resolution_clock::now();

	auto start = time_point_cast<microseconds>(this->startTimepoint).time_since_epoch().count();
	auto end = time_point_cast<microseconds>(this->endTimepoint).time_since_epoch().count();*/
	this->endTimepoint = MPI_Wtime();

	this->duration = this->endTimepoint - this->startTimepoint;
}

double MPITimer::getDuration() {
	return this->duration * 1000;
}