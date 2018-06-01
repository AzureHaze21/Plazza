/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _PROCESS_HH_
#define _PROCESS_HH_

#include <cstdlib>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "Timer.hh"
#include "ThreadPool.hh"
#include "IProcess.hh"
#include "Task.hh"
#include "Order.hh"
#include "Scraper.hh"
#include "Logger.hh"
#include "poll.h"

class Process : public IProcess
{
private:
	pid_t id = -1;
	std::size_t nThreads = 0;
	std::vector<std::future<void>> results;

public:
	explicit Process(std::size_t n) {
		nThreads = n;			
	}
	
	Process(Process &o) = delete;
	Process operator=(Process& o) = delete;
	
	virtual ~Process() {
		terminate();
	}
	
	void start(std::vector<Order>&& orders) override {

		id = fork();
		if (id == -1) {
			Logger::LOG(Logger::LOG_ERROR,
				    Color(RED, "void Process::start(): Failed to create child process"));
			throw std::runtime_error("Failed to create child process.");
		}
		
		if (id == 0) {
			if (orders.empty())
				terminate();
			{
				ThreadPool pool;
				pool.start(nThreads);					
				Scraper scraper;

				Timer<> t;
				std::size_t total = 0;
				for (const auto& o : orders) {
					for (const auto& f : o.files) {
						results.emplace_back(pool.addTask([&] {
									scraper.find(f, o.type);
								})
							);
						total++;
					}
				}
				
				for (auto& r : results)
					r.get();
				
				Logger::LOG(Logger::LOG_NORMAL, 
					    Color(GREEN,
						  std::to_string(total) + " file(s) in " +
						  std::to_string(orders.size()) + " orders treated in " +
						  std::to_string(t.elapsed()) + "ms"));
			}
			terminate();
		}
	}
	
        void wait() override {

		if (id > 0)
			waitpid(id, NULL, 0);
	}
	
	void terminate() noexcept override {

		if (id == 0)
			exit(0);
		else if (id > 0)
			kill(id, SIGKILL);
	}

	bool isBusy() {

		int st;
		int ret = waitpid(id, &st, WNOHANG);

		return (ret != -1);
	}

	pid_t getId() const {

		return id;
	}
};

#endif
