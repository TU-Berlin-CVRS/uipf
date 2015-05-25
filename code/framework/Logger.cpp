#include "Logger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace uipf
{

Logger* Logger::instance_ = 0;

Logger* Logger::instance()
{
  static Guard w;
  if( instance_ == 0 )
    instance_ = new Logger();
  return instance_;
}

void Logger::Warn(const std::string& strMessage)
{
	std::stringstream output;
	output << "[WARNING] " << strMessage << std::endl;
	print(output);
}

void Logger::Error(const std::string& strMessage)
{
	std::stringstream output;
	output << "[ERROR] " << strMessage << std::endl;
	print(output);
}

void Logger::Info(const std::string& strMessage)
{
	std::stringstream output;
	output << "[INFO] " << strMessage << std::endl;
	print(output);
	
}

void Logger::print(const std::stringstream& what)
{
	std::cout << what.str();
}

}

