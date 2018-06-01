//
// EPITECH PROJECT, 2018
// 
// File description:
// 
//

#ifndef _SCRAPER_HH_
#define _SCRAPER_HH_

#include <vector>
#include <string>
#include <map>
#include <regex>
#include <iostream>
#include <fstream>

#include "Color.hh"
#include "Order.hh"
#include "Logger.hh"
#include "Fifo.hh"

static std::map<OrderType, std::string> rgxpMap{
	{TYPE_EMAIL,	"(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"},
	{TYPE_PHONE,	"([0-9][0-9] ?.?){5}"},
	{TYPE_IP,	"(?:(?:0|1[\\d]{0,2}|2(?:[0-4]\\d?|5[0-5]?|[6-9])?" \
			"|[3-9]\\d?)\\.){3}(?:0|1[\\d]{0,2}|2(?:[0-4]\\d?|" \
			"5[0-5]?|[6-9])?|[3-9]\\d?)|(([0-9a-fA-F]{1,4}:)" \
			"{7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|" \
			"([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|"	\
			"([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|" \
			"([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|" \
			"([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|" \
			"([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|" \
			"[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|"	\
			":((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]" \
			"{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}" \
			"((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}" \
			"(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]"	\
			"{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])" \
			"\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))"}
};

class Scraper
{
private:
	Fifo out;
public:
	Scraper() : out(".plazza_data", Fifo::WRITE) {}
	virtual ~Scraper() {}

        bool find(std::string file, OrderType type)
		{
			if (rgxpMap.find(type) == rgxpMap.end()) {
				Logger::LOG(Logger::LOG_WARNING,
					    "Regexp not defined for this type");
				return false;
			}
			
			std::regex rgxp(rgxpMap[type]);
			std::ifstream iss(file);
			if (!iss) {
				Logger::LOG(Logger::LOG_ERROR,
					    "Could not open file '" + file + "' for reading");
				return false;
			}

			if (!out) out.reopen();
			
			std::string line;
			std::sregex_iterator end;
			std::size_t total = 0;
			while (std::getline(iss, line)) {
				std::sregex_iterator it(line.begin(), line.end(), rgxp);
				if (it != end) {
					if (out)
						out.write((*it)[0]);
					else
						std::cout << (*it)[0] << std::endl;
					Logger::LOG_RESULT((*it)[0]);
				        total++;
				}
			}

			Logger::LOG(Logger::LOG_NORMAL,
				    std::to_string(total) + " results found in '" + file + "'");

			return true;
		}
};

#endif
