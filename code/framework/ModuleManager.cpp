#include "ModuleManager.hpp"
#include "Configuration.hpp"

// loads the configuration and stores in the ModuleManager
/* 
conf	Configuration file, which has to be executed
*/
void ModuleManager::load(Configuration& conf){
	config = conf;
}

// executes the Configuration file
/*
*/
void ModuleManager::run(){
	// TODO
}
