#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <chrono>

class Debug
{
public:
	static void TimeProcess(std::string processName,std::function<void()> process)
	{
		auto start = std::chrono::high_resolution_clock::now();

		process();

		auto stop = std::chrono::high_resolution_clock::now();

		std::cout << processName << ":" << (std::chrono::duration_cast<std::chrono::microseconds>(stop - start)).count() << "mic. : " <<
			(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)).count() << "mil. : " << std::endl;
	}
};