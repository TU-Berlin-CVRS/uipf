#include "GUIEventDispatcher.h"

GUIEventDispatcher::GUIEventDispatcher() {}

GUIEventDispatcher::~GUIEventDispatcher() {}


void GUIEventDispatcher::triggerReportProgress(const float& value)
{
	emit reportProgressEvent(value);
}

