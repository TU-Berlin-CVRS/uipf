#include "ModuleBase.hpp"

using namespace std;

namespace uipf
{

ModuleBase::ModuleBase()
:context_(nullptr)
{
}

ModuleBase::~ModuleBase()
{
}

void ModuleBase::setContext(Context* context)
{
	context_ = context;
}

void ModuleBase::listParams( map < string, string >& params) const
{
 Logger::instance()->Info("Module has been executed with the following params:");
  
  map < string, string >::iterator it = params.begin ();
  for (; it != params.end (); ++it)
    {
	Logger::instance()->Info("    " + it->first + " - " + it->second );
    }
}


}

