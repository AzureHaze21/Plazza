/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _ORDER_HH_
#define _ORDER_HH_

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "Logger.hh"

enum OrderType {
	TYPE_UNKNOWN = 0,
	TYPE_EMAIL,
	TYPE_PHONE,
	TYPE_IP
};

enum OrderStatus {
	ST_INVALID,
	ST_CREATED,
	ST_PENDING,
	ST_DONE
};

static const std::map<std::string, OrderType> orderTypeNames{
	{"EMAIL_ADDRESS",	TYPE_EMAIL},
	{"PHONE_NUMBER",	TYPE_PHONE},
	{"IP_ADDRESS",		TYPE_IP}
};

struct Order
{	
	std::vector<std::string> files;
	std::string strType = "";
	OrderType type;
	OrderStatus status;

        explicit Order(std::string&& str) :
		type(TYPE_UNKNOWN), status(ST_INVALID) 
		{
			std::stringstream ss(std::move(str));
			std::vector<std::string> words;
			std::string w;

			while (ss >> w)
				words.push_back(w);
		      
			if (words.size() < 2) {
				Logger::LOG(Logger::LOG_WARNING, "Invalid order '"+str+"'");
				return;
			}

			std::string last = std::move(words.back());
			auto type = orderTypeNames.find(last);

			if (type == orderTypeNames.end()) {
				Logger::LOG(Logger::LOG_WARNING,
					    std::string("Unknown order type '"+last+"'"));
				return;
			}

			strType = last;
			words.pop_back();
			files = std::move(words);
			this->type = type->second;
			status = ST_CREATED;
		}
	
	virtual ~Order() {}

	std::string toString()
		{
			if (status == ST_INVALID) {
				return std::string("{{},}");
			}

			std::string str("{{");

			for (auto& f : files) {
				if (f != files.front()) {
					str += ", ";
				}
				str += "\"" + f + "\"";
			}
			str += "}, " + strType + "}";
			return (str);
		}

	operator bool()
		{
			return (status != ST_INVALID);
		}
};

#endif
