/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _SCHEDULER_HH_
#define _SCHEDULER_HH_

#include <vector>

#include "Process.hh"
#include "IProcess.hh"
#include "Order.hh"

class Scheduler
{
private:
	std::size_t totalOrders{0};
	std::size_t threadsPerProc{0};
	std::size_t instances{0};
	std::size_t pending{0};
	
	std::vector<std::unique_ptr<Process>> procs;

public:
	explicit Scheduler(std::size_t tpp)
		{
			threadsPerProc = tpp > 0 ? tpp : 1;
		}

	virtual ~Scheduler()
		{
			stop();
		}

	void start()
		{
			
		}

	void stop() noexcept
		{
			for (auto& p : procs) {
				(*p).terminate();
			}
		}
	
	void dispatch(std::vector<Order>&& orders)
		{
			if (orders.empty())
				return;

			std::size_t nProc;			
			if (orders.size() <= threadsPerProc * 2)
				nProc = 1;
			else
				nProc = 1 + (orders.size() / (threadsPerProc * 2));
			
			if (nProc > procs.size()) {
				std::size_t sz = procs.size();
				for (std::size_t i = 0; i < nProc - sz; i++)
					procs.push_back(std::make_unique<Process>(threadsPerProc));
			}

			auto itStart = orders.begin();
			auto itEnd = orders.begin();
			for (auto& p : procs) {
				for (std::size_t i = 0; i++ < threadsPerProc * 2 && itEnd != orders.end(); itEnd++) {}
				std::vector<Order> tmp(itStart, itEnd);
				(*p).start(std::move(tmp));
				itStart = itEnd;
				if (itStart == orders.end())
					break;
			}
			
		        Logger::LOG(Logger::LOG_NORMAL,
				    std::to_string(orders.size())
				    + " order(s) dispatched between " +
				    std::to_string(nProc)
				    + " processe(s) and " +
				    std::to_string(nProc * threadsPerProc)
				    + " threads");
		}
};

#endif
