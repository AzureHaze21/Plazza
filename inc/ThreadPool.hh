/*
** EPITECH PROJECT, 
** 
** (c) 2018 by Paul Young (paul.young@epitech.eu)
** 
*/

#ifndef _THREADPOOL_HH_
#define _THREADPOOL_HH_

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <future>
#include <iostream>
#include <functional>

#include "Task.hh"

class ThreadPool
{
private:
	std::vector<std::thread> workers;
	std::queue<Task> tasks;
	std::condition_variable mTaskEvent;
	std::mutex mtx;
        bool hasFinished = false;
	
public:
        ThreadPool() { }
	virtual ~ThreadPool() { stop(); };

	void start(std::size_t nthreads) {
		for (std::size_t i = 0; i < nthreads; ++i) {
			workers.emplace_back([=] {
					while (true) {
						Task t;
						{
							std::unique_lock<std::mutex> lock(mtx);
							mTaskEvent.wait(lock, [=] {
									return !tasks.empty() || hasFinished;
								});
							if (hasFinished && tasks.empty())
								break;
							t = std::move(tasks.front());
							tasks.pop();
						}
						t();
					}
				});
		}

	}

	template<typename T>
	auto addTask(T task)->std::future<decltype(task())> {
		auto pTask = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
		{
			std::unique_lock<std::mutex> lock(mtx);
			tasks.emplace([=] { (*pTask)(); });
		}
		mTaskEvent.notify_one();
		return (pTask->get_future());
	}

private:

	void stop() noexcept {
		{
			std::lock_guard<std::mutex> lock(mtx);
			hasFinished = true;
			mTaskEvent.notify_all();
		}	
		for (auto &w : workers) { w.join(); }
	}
};

#endif
