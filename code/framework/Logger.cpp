#include "Logger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace uipf
{

Logger* Logger::instance_ = nullptr;

Logger* Logger::instance()
{
  static Guard w;
  if( instance_ == nullptr )
    instance_ = new Logger();
  return instance_;
}

void Logger::Warn(const std::string& strMessage)
{
	std::stringstream output;
	output << "[WARNING] " << strMessage << std::endl;
	print(output);

	//send signal to GUI
	emit logEvent(WARNING,strMessage);
}

void Logger::Error(const std::string& strMessage)
{
	std::stringstream output;

	output << "\033[1;31m[ERROR] " << strMessage << "\033[0m" << std::endl;//output in red using ANSI codes (linux only): 
									       //http://en.wikipedia.org/wiki/ANSI_escape_code
	print(output);

	//send signal to GUI
	emit logEvent(ERROR,strMessage);
}

void Logger::Info(const std::string& strMessage)
{
	std::stringstream output;
	output << "[INFO] " << strMessage << std::endl;
	print(output);

	//send signal to GUI
	emit logEvent(INFO,strMessage);
}

void Logger::print(const std::stringstream& what)
{

	std::cout << what.str();
}

}

