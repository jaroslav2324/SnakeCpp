#pragma once

#include <iostream>

#include "SDL.h"

using std::cout;
using std::endl;

class Timer {
public:
	Timer(const double countPeriod) : countPeriod(countPeriod) {
		currentTime = SDL_GetTicks();
	};
	void printCountPeriod();
	void printAmountTicks();

	bool tickIfNeeded();
protected:
	// in ms
	double prevTime = 0;
	double currentTime;
	double frameTime = 0;
	const double countPeriod;

	long long amountTicks = 0;
};

/*return true if tick has been made otherwise false*/
bool Timer::tickIfNeeded() {

	prevTime = currentTime;
	currentTime = SDL_GetTicks();

	double deltaTime = currentTime - prevTime;

	frameTime += deltaTime;

	if (frameTime >= countPeriod) {
		frameTime -= countPeriod;
		amountTicks++;
		return true;
	}

	return false;
}

void Timer::printAmountTicks() {

	cout << "Amount ticks: " << amountTicks << endl;
}

void Timer::printCountPeriod() {

	cout << "Count period: " << countPeriod << endl;
}
