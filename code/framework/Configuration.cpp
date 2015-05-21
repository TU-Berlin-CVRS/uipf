#include "Configuration.hpp"
#include "ModuleConfig.hpp"

// 
/*
*/
void Configuration::load(string filename){

	//~ while(no end of yaml result){
		ModuleConfig modConfig;
		// use yaml to create the modConfig
		chain.push_back(modConfig); 
	//~ }
}

// 
/*
*/
void Configuration::store(string filename){
	// TODO
}

