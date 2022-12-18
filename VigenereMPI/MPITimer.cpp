#include "mpi_timer.h"

MPITimer::MPITimer(){
	this->duration = 0.0;
	this->startTimepoint = 0.0;
	this->endTimepoint = 0.0;
}

void MPITimer::start() {
	this->startTimepoint = MPI_Wtime();
}

void MPITimer::stop() {
	this->endTimepoint = MPI_Wtime();

	this->duration = this->endTimepoint - this->startTimepoint;
}

double MPITimer::getDuration() {
	return this->duration * 1000;
}