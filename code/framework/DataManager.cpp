#include "DataManager.hpp"
#include "Logger.hpp"

using namespace std;
using namespace uipf;

void DataManager::listParams( ) const
{
  LOG_I("Module has been executed with the following params:");

  map < string, string >::iterator it = params_.begin ();
  for (; it != params_.end (); ++it)
  {
    LOG_I("    " + it->first + " - " + it->second );
  }
}

// check whether named input data is given
bool DataManager::hasInputData( const std::string& strName) const {
	return input_.find(strName) != input_.end();
}
