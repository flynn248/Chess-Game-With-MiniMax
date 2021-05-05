//For basic benchmarking purposes
#include <iostream>
#include <memory>
#include <chrono>
#ifndef TIMER_H
#define TIMER_H
//can use this to output the information into a file
class Timer 				{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;


public:
	Timer() {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		stop();
	}

	void stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;
		double s = ms * 0.001;
		std::cout << duration << "us (" << ms << "ms) " << s << "s\n";
	}
};

#endif // !TIMER

