/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _PLAZZACORE_HH_
#define _PLAZZACORE_HH_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <future>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <poll.h>

#include "Order.hh"
#include "Color.hh"
#include "Line.hh"
#include "Fifo.hh"
#include "Scheduler.hh"
#include "Logger.hh"

using namespace std::chrono_literals;

class PlazzaCore
{
private:
	std::atomic<bool> isRunning{true};
	std::unique_ptr<Scheduler> pScheduler;
	std::vector<Order> orders;
	std::condition_variable condInputEvent;
	std::mutex mtx;

	const std::string prompt = "[reception@plazza]: ";

public:
	explicit PlazzaCore(std::size_t tpp) : pScheduler(std::make_unique<Scheduler>(tpp)) { }
	
	virtual ~PlazzaCore() {
		stop();
		Logger::LOG(Logger::LOG_NORMAL, "logfile stopped");
	}

	void start() {
		Logger::LOG(Logger::LOG_NORMAL, "logfile started");

		std::thread tDisplay([&] {
				Fifo in(".plazza_data", Fifo::READ);
				std::string line;
				while (isRunning) {
					line = in.readLine();
					if (line.size() > 0)
						std::cout << line << std::endl;
					else
						in.reopen();
				}
			});
	        
		std::thread tSheduler([&] {
				while (isRunning) {
					{
						std::unique_lock<std::mutex> lock(mtx);
						condInputEvent.wait(lock, [&] {
								return (!isRunning || !orders.empty());
							});
						if (!isRunning || orders.empty())
							return;
						pScheduler->dispatch(std::move(orders));
						orders.clear();
						condInputEvent.notify_one();
					}
				}
			});
			
		//std::ifstream ifs("/dev/tty", std::ifstream::in);			
		while (true) {
			/*
			if (!std::cin) {
				if (!ifs)
					return;
				std::cin.rdbuf(ifs.rdbuf());
				std::cin.clear();
				Logger::LOG(Logger::LOG_NORMAL, "Input switched to terminal");
			}
			*/
			std::string line;
			std::getline(std::cin, line);
			if (!line.compare("exit")) {
				isRunning = false;
				condInputEvent.notify_one();
				std::exit(0);
			}

			std::istringstream iss(line);
			for (std::string s; std::getline(iss, s, ';'); ) {
				Order o(std::move(s));
				if (o)
				{
					{
						std::unique_lock<std::mutex> lock(mtx);
						orders.push_back(o);
						condInputEvent.notify_one();
					}
				}
			}
		}

		tDisplay.join();
		tSheduler.join();
	}

	void stop() noexcept {
		isRunning = false;
		pScheduler->stop();
	}
};

#endif
