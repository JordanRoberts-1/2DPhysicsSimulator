#pragma once
#include <chrono>



class Clock
{
public:
	static double CurrTimeInMillis()
	{
		auto now = std::chrono::high_resolution_clock::now();
		double ms = std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count() / MICROSECONDS_PER_MS;
		return ms;
	}

private:
	const static double MICROSECONDS_PER_MS;
};

const double Clock::MICROSECONDS_PER_MS = 1000.0;
