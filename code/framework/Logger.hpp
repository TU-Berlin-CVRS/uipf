#ifndef LOGGER_H
#define LOGGER_H

//shortcuts to log
//can be used like LOG_E("something bad happened!")
#define LOG_E(message) Logger::instance()->Error(message)
#define LOG_I(message) Logger::instance()->Info(message)
#define LOG_W(message) Logger::instance()->Warn(message)

#include <sstream>
#include <QObject>

namespace uipf
{


//This is a very simple Logger implemented as singleton, that writes to console
//2DO: use a loggingframework e.g. Log4QT
class Logger : public QObject
{
Q_OBJECT

	public:
		enum LogType
		{
			INFO,WARNING,ERROR
		};
	public:
     		static Logger* instance();

	   private:
	     static Logger *instance_;
	     Logger() {}
	     Logger( const Logger& );

	     ~Logger() {}

	 	class Guard {
			 public: ~Guard() {
			   if( Logger::instance_ != 0 )
			     delete Logger::instance_;
			 }
		     };
     		friend class Guard;

	public:
			
		void Warn(const std::string& strMessage);
		void Error(const std::string& strMessage);
		void Info(const std::string& strMessage);

	signals:
		void logEvent(const Logger::LogType&, const std::string&);

	private:
		
		void print(const std::stringstream& what);
		
};

} //namespace

#endif // LOGGER_H

