#pragma once
class AbstractTimer {
public:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual double getDuration() = 0;
};