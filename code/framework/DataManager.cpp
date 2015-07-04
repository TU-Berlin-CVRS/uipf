#include "DataManager.hpp"
#include "Logger.hpp"

using namespace std;

namespace uipf
{

void DataManager::listParams( ) const
{
  LOG_I("Module has been executed with the following params:");

  map < string, string >::iterator it = params_.begin ();
  for (; it != params_.end (); ++it)
  {
    LOG_I("    " + it->first + " - " + it->second );
  }
}


}
