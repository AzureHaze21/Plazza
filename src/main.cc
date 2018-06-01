//
// EPITECH PROJECT, 2018
// 
// File description:
// 
//

#include <iostream>

#include "PlazzaCore.hh"
#include "Logger.hh"

static const char usage[] = "USAGE: ./plazza <n>";

int main(int argc, char **argv)
{
       	if (argc != 2) {
		std::cerr << usage << std::endl;
		return (-1);
	}
	try {
		int n = std::stoi(argv[1]);

		if (n < 1)
			throw std::out_of_range("n out of range");
		
		PlazzaCore core(n);
		core.start();

	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		Logger::LOG(Logger::LOG_ERROR, "Launch failed: " + std::string(e.what()));
		return (-1);
	}
	return 0;
}
