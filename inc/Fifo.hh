/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _FIFO_HH_
#define _FIFO_HH_

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Order.hh"

class Fifo
{
public:
	enum e_mode
	{
		READ = 0,
		WRITE,
		RDWR
	};

	std::string path;
	int fd;
	bool good = true;
	e_mode mode;

public:
	
	explicit Fifo(std::string&& name, e_mode m)
		: path(std::move(name)), mode(m) {
		::mkfifo(path.c_str(), 0644);
		fd = open(path.c_str(),
			  m == READ ? O_RDONLY
			  : m == WRITE ? O_WRONLY
			  : O_RDWR | O_NONBLOCK);
	}

	virtual ~Fifo() {
		::close(fd);
	}

	void reopen() {
		close(fd);
		fd = open(path.c_str(), mode);
	}
	
        void destroy() noexcept {
		
	}

        int write(std::string msg) {
		std::string str = msg + '\n';
		int ret = ::write(fd, str.c_str(), str.size());
		return (ret);			
	}

        std::string readSome(std::size_t n) {
		char buff[n] = { 0 };
		
		::read(fd, buff, n);
		return (std::string(buff));
	}
	
	std::string readLine() {
		std::string str("");
		char c;

		while (::read(fd, &c, 1) > 0) {
			if (c == '\n')
				break;
			str += c;
		}
		return (str);
	}
	
	std::vector<Order> readOrders()	{
		std::string tmp;
		std::vector<Order> ret;
		char buff;
		
		while (::read(fd, &buff, 1) > 0) {
			if (buff == ';')
				tmp = "";
			else
				tmp += buff;
		}
		return (ret);
	}
	
	operator bool() {
		return (good);
	}
};

#endif
