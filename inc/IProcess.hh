/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _IPROCESS_HH_
#define _IPROCESS_HH_

#include <vector>

#include "Task.hh"
#include "Order.hh"


class IProcess
{
protected:
	enum e_ProcState {
		CREATED = 0,
		READY,
		RUNNING,
		BLOCKED,
		TERMINATED
	} procState;

public:
	IProcess() : procState(CREATED) {}
	virtual ~IProcess() {}

	virtual void start(std::vector<Order>&&) = 0;
	virtual void wait() = 0;
	virtual void terminate() = 0;
};

#endif
