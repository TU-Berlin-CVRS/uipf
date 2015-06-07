#include <iostream>
#include <string>
#include <QApplication>
#include "framework/ModuleManager.hpp"
#include "framework/Configuration.hpp"

// foor Boost:
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace uipf;

// argument is a configFile
int main(int argc, char** argv){

	if (argc < 2) {
		std::cerr << "Usage Error, at least one argument is required!\n\n";
		std::cerr << "Usage: " << argv[0] << " <configFile>"<< std::endl;
		return 1;
	}
	
	ModuleManager mm(conf);
	
	// run a processing chain from a config file.
	// ./uipf -c example.yam
	if (string(argv[1]).compare("-c") == 0){
		
		
		// QUESTION: for what is this used??
		QApplication app (argc,argv);

		// loads the configFile and create a Configuration
		Configuration conf;
		string configFileName = argv[2];
		conf.load(configFileName);

		// only for debug, print the loaded config
		conf.store("test.yaml");
		
	} else{
	// run a single module.
	// ./uipf <moduleName> ...options...		
	// TODO finish it

	// loads the configFile and create a Configuration
		Configuration conf;
		
		ProcessingStep processModule;
		processModule.name = "processModule";
		processModule.module = argv[2];	
		
		try {
			
			// Declare three groups of options.
				// Declare a group of options that will be allowed only on command line
				po::options_description generic("Generic options");
				generic.add_options()
					("version,v", "print version string")
					("help", "produce help message")    
					;
					
				// Declare a group of options that will be 
				// allowed both on command line and in
				// config file
				po::options_description config("Configuration");
				config.add_options()
					("-mn,--modulename", po::value< string>(), "defines the name of the module")
					("-i,--input", po::value< vector<string> >()->composing(), "defines an input, can be used multiple times \n format: inputName:fileName \n inputName is optional if there is only one input")
					("-o,--output",	po::value< vector<string> >()->composing(), "defines an output, can be used multiple times \n format: outputName:fileName \n outputName is optional if there is only one output \n output itself is optional, if there is only one input and one output, \n the output filename will be choosen from the input name in this case.")
					("-p,--param",	po::value< vector<string> >()->composing(),	"defines a parameter, format:  name:valu")
					;

				// Hidden options, will be allowed both on command line and
				// in config file, but will not be shown to the user.
				po::options_description hidden("Hidden options");
				hidden.add_options()
					("-mn", po::value< string>(), "module name")
					;    
			
			// These two lines say that all positional options should be translated into "-modulename" options. 
			po::positional_options_description p;
			p.add("-mn", -1);
			
			// Declare an options description instance which will include all the options
			po::options_description all("Allowed options");
			all.add(generic).add(config).add(hidden);

			// Declare an options description instance which will be shown to the user
			po::options_description visible("Allowed options");
			visible.add(generic).add(config);
			

			po::variables_map vm;
			store(parse_command_line(argc, argv, all), vm);
			po::notify(vm);
		
			if (vm.count("version")) {
				cout << "Version: 1.0" << "\n";
				return 0;
			}
				  
			if (vm.count("help")) {
				cout << visible;
				return 0;
			}
			
			
			if (vm.count("-i")) {
				// this step is repeated as often, as the number of load modules is created 
				for (int i=0; i<vm["-i"].size(); i++){
					ProcessingStep loadModule;
					loadModule.name = "loadModule" + to_string(i);
					// TODO: check whether it is the correct name of the module and correct name of the param
					loadModule.module = "loadImage";
					loadImage.params.insert (pair<string,string>("filename",vm["-i"][i].as<string>()) );
					
					conf.addProcessingStep(loadModule);
				} 
				
				// if only one input create an output automatically
				if (vm["-i"].size() == 1){
					string modName = vm["-mn"];
					MetaData md = mm.getModuleMetaData(modName);
					map<string, Type> out = md.getOutputs();
					if(map.size ==1){
					
						string outName = out[0].first;
						
						map<string, string> storeSource;
						storeSource.insert(pair<string, string>(modName,storeSource) );
						
						
						ProcessingStep storeModule;
						storeModule.name = "storeModule";
						// TODO: check whether it is the correct name of the module and correct name of the param
						storeModule.module = "storeImage";	
						
						
						storeImage.input.insert (pair<string, pair<string, string> >("image", storeSource));
						storeImage.params.insert (pair<string,string>("filename",rename(vm["-i"][i].as<string>()))); 		

						conf.addProcessingStep(storeModule);
					}
				}
			}


			if (vm.count("-o")) {
				
				// delete the automatically created output
				conf.removeProcessingStep(storeModule);
				
				
				string modName = vm["-mn"];
				MetaData md = mm.getModuleMetaData(modName);
				map<string, Type> out = md.getOutputs();
					
				// this step is repeated as often, as the number of store modules is created 
				for (int i=0; i<vm["-o".size()]; i++){
				
					string outName = out[i].first;
					
					map<string, string> storeSource;
					storeSource.insert(pair<string, string>(modName,storeSource) );
								
					ProcessingStep storeModule;
					storeModule.name = "storeModule" + to_string(i);
					// TODO: check whether it is the correct name of the module and correct name of the param
					storeModule.module = "storeImage";	
					storeImage.input.insert (pair<string, pair<string, string> >("image",storeSource) );
					storeImage.params.insert (pair<string,string>("filename",vm["-o"][i].as<string>()) ); 

					conf.addProcessingStep(storeModule);
					
				} 
			}
			
			
			if (vm.count("-p")) {

				string modName = vm["-mn"];
				MetaData md = mm.getModuleMetaData(modName);
				map<string, Type> out = md.getParams();
					
				// this step is repeated as often, as the number of store modules is created 
				for (int i=0; i<vm["-o".size()]; i++){
				
					string outName = out[i].first;
					
					map<string, string> storeSource;
					storeSource.insert(pair<string, string>(modName,storeSource) );
								
					ProcessingStep storeModule;
					storeModule.name = "storeModule" + to_string(i);
					// TODO: check whether it is the correct name of the module and correct name of the param
					storeModule.module = "storeImage";	
					storeImage.input.insert (pair<string, pair<string, string> >("image",storeSource) );
					storeImage.params.insert (pair<string,string>("filename",vm["-o"][i].as<string>()) ); 

					conf.addProcessingStep(storeModule);
					
				} 

				process.input.insert (pair<string, pair<string, string> >(???,???) ); 	// fill from -i


				process.params.insert (pair<string,string>("filename",???) ); 			// fill from -p

			}
			
			conf.addProcessingStep(processModule);			

		}
		catch(exception& e) {
			cout << e.what() << "\n";
			return 1;
		}

	}
	
	// loads the Configuration and runs it
	mm.run();	
	
	
	return 0;
}

// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
/*
s	old name, which has to be renamed 
*/
string rename(string s){

	istringstream iss(s);
	
	vector<std::string> tokens;
	string token;
	
	while (getline(iss, token, '.')) {
		if (!token.empty())
			tokens.push_back(token);
	}
	
	string end = tokens.[tokens.size()-1];
	tokens.push_back(_result);
	tokens.push_back(end);
	
	string newName;
	
	newName = tokens[0];
	for (int i=1; i<tokens.size(); i++){
		newName = newName +"." + tokens[i];
	}
	
	return newName;
}
