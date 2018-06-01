/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#ifndef _LOGGER_HH_
#define _LOGGER_HH_

#include "Color.hh"

namespace Logger
{
	enum LogType
	{
		LOG_NORMAL,
		LOG_WARNING,
		LOG_ERROR
	};

	class Logger
	{
	private:
		std::mutex writeMutex;
		std::ofstream file;
		
	public:
		
		Logger(std::string logFile) { file.open(logFile, std::ios::out | std::ios::app); }
		virtual ~Logger() { file.close(); }

		void log(std::string&& msg)
			{
				{
					std::lock_guard<std::mutex> lock(writeMutex);
					file << msg << std::endl;
				}
			}
			
		void log(LogType type, std::string&& msg)
			{
				{
					std::lock_guard<std::mutex> lock(writeMutex);
						
					switch (type) {
					case LOG_NORMAL:
						file << "[" << __TIME__ << "][INFO]: "
						     << msg << std::endl; break;
					case LOG_WARNING:
						file << Color(ORANGE, "[" + std::string(__TIME__) + "][WARNING]: " + msg)
						     << std::endl; break;
					case LOG_ERROR:
						file << Color(RED, "[" + std::string(__TIME__) + "][ERROR]: " + msg)
						     << std::endl; break;
					}
				}
			}
			
		operator bool() { if (file) { return true;  } return false; }
	};

	static void LOG(LogType type, std::string&& msg)
	{
		static Logger logs("logs.txt");
		
		if (logs)
			logs.log(type, std::move(msg));
       	}

	static void LOG_RESULT(std::string&& msg)
	{
		static Logger resultsLog("results.txt");

		if (resultsLog)
			resultsLog.log(std::move(msg));
	}
};

#endif
