#include <iostream>
#include <string>
#include <QApplication>
#include "framework/ModuleManager.hpp"
#include "framework/Configuration.hpp"
#include "framework/Utils.hpp"
#include "framework/Logger.hpp"

// foor Boost:
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace uipf;


// argument is a configFile
int main(int argc, char** argv){

	// Declare a group of options that will be allowed only on command line
	po::options_description generic("Generic options");
	generic.add_options()
		("version,v", "print version string")
		("help,h", "produce help message")
		;

	po::options_description config("Program options");
	config.add_options()
		("configuration,c", po::value< string >(), "defines the path to an already created and stored chain configuration, written in yaml")
		("input,i", po::value< vector<string> >()->composing(), "defines an input, can be used multiple times format: inputName:fileName inputName is optional if there is only one input")
		("output,o",	po::value< vector<string> >()->composing(), "defines an output, can be used multiple times, format: outputName:fileName. outputName is optional, if there is only one input. Output is optional, if there is only one input and one output, the output filename will be chosen from the input name in this case.")
		("param,p",	po::value< vector<string> >()->composing(),	"defines a parameter, format:  name:value")
		;

	// Hidden options, will be allowed command line, but will not be shown to the user.
	po::options_description hidden("Hidden options");
	hidden.add_options()
		("modulename,m", po::value< string>(), "defines the name of the module")
		;

	// These two lines say that all positional options should be translated into "-modulename" options.
	po::positional_options_description p;
	p.add("modulename", -1);

	// Declare an options description instance which will include all the options
	po::options_description allOptions("Allowed options");
	allOptions.add(generic).add(config).add(hidden);

	// Declare an options description instance which will be shown to the user
	po::options_description visibleOptions("Allowed options");
	visibleOptions.add(generic).add(config);

	// process input parameters and store the result in vm
	po::variables_map vm;
	store(po::command_line_parser(argc, argv).options(allOptions).positional(p).run(), vm);
	po::notify(vm);



	if (argc < 2) {
		std::cerr << "Usage Error!\n\n";
		cout << "Usage:" << endl;
		cout << argv[0] << " -c <path to configuration file>				run a processing chain from a config file." << endl;
		cout << argv[0] << " <moduleName> -i <input> [-p <params> -o <output>]	run a single module." << endl;
		cout << "\n";
		// show help
		cout << visibleOptions;
		return 1;
	}

	// used by the module manager to access Qt components, TODO maybe move to module manager?
	QApplication app (argc,argv);

	ModuleManager mm;
	Configuration conf;

	if (vm.count("configuration")){
	// run a processing chain from a config file.
	// ./uipf -c example.yam

		// loads the configFile and create a Configuration
		string configFileName = argv[2];
		conf.load(configFileName);


	} else{
	// run a single module.
	// ./uipf <moduleName> ...options...

		if (vm.count("version")) {
			cout << "Version: 1.0" << "\n";
			return 0;
		}

		if (vm.count("help")) {
			cout << "Usage:" << endl;
			cout << argv[0] << " -c <path to configuration file>				run a processing chain from a config file." << endl;
			cout << argv[0] << " <moduleName> -i <input> [-p <params> -o <output>]	run a single module." << endl;
			cout << "\n";
			cout << visibleOptions;
			return 0;
		}

		if (!vm.count("modulename") || !vm.count("input")){
			std::cerr << "Usage Error!\n\n";
			cout << "Usage:" << endl;
			cout << argv[0] << " -c <path to configuration file>				run a processing chain from a config file." << endl;
			cout << argv[0] << " <moduleName> -i <input> [-p <params> -o <output>]	run a single module." << endl;
			cout << "\n";
			// show help
			cout << visibleOptions;
			return 1;
		}

		string modName = vm["modulename"].as<string>();
		if (!mm.hasModule(modName)) {
			LOG_E("Module " + modName + " does not exist!");
			return 1;
		}
		MetaData md = mm.getModuleMetaData(modName);

		ProcessingStep processModule;
		processModule.name = "processModule";
		processModule.module = modName;

		if (vm.count("input")) {
			vector<string> inputs = vm["input"].as< vector<string> >();
			// this step is repeated as often, as the number of load modules is created
			for (unsigned int i=0; i<inputs.size(); i++){
				ProcessingStep loadModule;
				loadModule.name = "loadModule" + to_string(i);
				loadModule.module = "loadImage";

				string source = utils::secondPart(inputs[i]);

				loadModule.params.insert (pair<string,string>("filename",source) );

				conf.addProcessingStep(loadModule);

				// the input params of the module are set
				map<string, DataDescription> in = md.getInputs();
				string name;
				// if there is only one input, the name is optional on the command line, will be taken from metadata
				if (in.size() == 1) {
					auto it = in.cbegin();
					name = it->first;
				} else {
					name = utils::firstPart(inputs[i]);
				}
				pair<string,string> loadWithValue(loadModule.name, "image");

				processModule.inputs.insert(pair<string, pair<string, string> >(name, loadWithValue));
			}

			// if only one input, create an output automatically, if none is given explicitly
			if (inputs.size() == 1 && vm.count("output") == 0){
				map<string, DataDescription> out = md.getOutputs();
				// if only one output exists
				if(out.size() ==1){

					auto it = out.cbegin();
					string outName = it->first;

					pair<string, string> storeSource(processModule.name,outName);

					ProcessingStep storeModule;
					storeModule.name = "storeModule";
					storeModule.module = "storeImage";

					// where does the image come from
					storeModule.inputs.insert (pair<string, pair<string, string> >("image", storeSource));

					// where should it be stored
					string newName = utils::rename(utils::secondPart(inputs[0]));
					storeModule.params.insert (pair<string,string>("filename", newName));

					conf.addProcessingStep(storeModule);
				}
			}

		}


		if (vm.count("output")) {

			map<string, DataDescription> out = md.getOutputs();

			vector<string> outputs = vm["output"].as< vector<string> >();
			// this step is repeated as often, as the number of store modules is created
			for (unsigned int i=0; i<outputs.size(); i++){

				// if there is only one output, the name is optional on the command line, will be taken from metadata
				string outName;
				if (out.size() == 1) {
					auto it = out.cbegin();
					outName = it->first;
				} else {
					outName = utils::firstPart(outputs[i]);
				}

				pair<string, string> storeSource(processModule.name, outName);

				ProcessingStep storeModule;
				storeModule.name = "storeModule" + to_string(i);
				storeModule.module = "storeImage";

				// where does the image come from
				storeModule.inputs.insert (pair<string, pair<string, string> >("image",storeSource) );
				// where should it be stored
				string storeName = utils::secondPart(outputs[i]);
				storeModule.params.insert (pair<string,string>("filename",storeName) );

				conf.addProcessingStep(storeModule);

			}
		}


		if (vm.count("param")) {

			vector<string> params = vm["param"].as< vector<string> >();
			// this step is repeated as often, as the number of params is inserted
			for (unsigned int i=0; i<params.size(); i++){

				processModule.params.insert (pair<string,string>(utils::firstPart(params[i]), utils::secondPart(params[i])));

			}
		}

		conf.addProcessingStep(processModule);
	}

	// print the loaded config
	LOG_I("Here is the loaded configuration:");
	conf.print();

	// validate configuration and show errors
	pair< vector<string>, vector<string> > errors = conf.validate(mm.getAllModuleMetaData());
	if (!errors.first.empty()) {
		LOG_E("There are configuration errors!");
		for(unsigned int i = 0; i < errors.first.size(); ++i) {
			LOG_E(errors.first[i]);
		}
		return 1;
	}

	// run the current configuration
	mm.run(conf);

	return 0;
}
