#include "GUIEventDispatcher.h"
#include "Logger.hpp"
#include <iostream>

//Register Types for interthreadcommunication. qRegisterMetaType() needs to be called too -> ctr()
Q_DECLARE_METATYPE(uipf::Logger::LogType)
Q_DECLARE_METATYPE(std::string)

namespace uipf
{

GUIEventDispatcher* GUIEventDispatcher::instance_ = nullptr;

GUIEventDispatcher* GUIEventDispatcher::instance()
{
  static GUIEventDispatcher::Guard w;
  if( instance_ == nullptr )
    instance_ = new GUIEventDispatcher();
  return instance_;
}

GUIEventDispatcher::GUIEventDispatcher()
{
	//All custom types (even that from std::), that should be used in signals and slots that are invoked from different threads
	//have to be registered for QT in their Typesystem
	qRegisterMetaType<uipf::Logger::LogType>("Logger::LogType");
	qRegisterMetaType<std::string>("std::string");
}

GUIEventDispatcher::~GUIEventDispatcher() {}


void GUIEventDispatcher::triggerReportProgress(const float& value)
{
	//send signal to GUI
	emit reportProgressEvent(value);
}

void GUIEventDispatcher::triggerLogEvent(const Logger::LogType& eLogType, const std::string& strMessage)
{
	//send signal to GUI
	emit logEvent(eLogType,strMessage);
}

} //namespace
