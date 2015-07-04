#ifndef LOGGER_H
#define LOGGER_H

//shortcuts to log
//can be used like LOG_E("something bad happened!")
#define LOG_E(message) Logger::instance()->Error(message)
#define LOG_I(message) Logger::instance()->Info(message)
#define LOG_W(message) Logger::instance()->Warn(message)
#define LOG_D(message) Logger::instance()->Debug(message)

#include <sstream>

namespace uipf
{

//This is a simple Logger implemented as singleton, that writes to console and sends messages to the GUI.
//Herefore it uses the GUIEventDispatcher.
//Messages are logged threadsafe. So it does not matter if e.g. LOG_I() is called in a GUI-Thread or from a Backgroundworker.
class Logger
{
	public:
		enum LogType
		{
			INFO,WARNING,ERROR,DEBUG
		};
	public:
		static Logger* instance();

	private:
		// holds the singleton instance of the logger
		static Logger *instance_;
		Logger() {}
		Logger( const Logger& );

		~Logger() {}

		// helper class to ensure logger gets deleted when the context is gone
		class Guard {
			public: ~Guard() {
				if( Logger::instance_ != 0 ) {
					delete Logger::instance_;
				}
			}
		};
		friend class Guard;

	public:
		//Warnings are colored blue in the GUI
		void Warn(const std::string& strMessage);
		//Errors are colored red in the GUI
		void Error(const std::string& strMessage);
		//Info are colored green in the GUI
		void Info(const std::string& strMessage);
		//Debugmessages can be used internally. They are shown when the project is built with CMAKE_BUILD_TYPE=Debug
		void Debug(const std::string& strMessage);

	private:

		void print(const std::stringstream& what);

};

} // namespace

#endif // LOGGER_H
