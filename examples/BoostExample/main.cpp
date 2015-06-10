#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std;



int main(int ac, char* av[])
{
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
			int opt;
			po::options_description config("Configuration");
			config.add_options()
				("optimization", po::value<int>(&opt)->default_value(10), "optimization level")
				//~ ("include-path,I", po::value< vector<string> >()->composing(), "include path")
				("include-path,I", po::value<string>()->composing(), "include path")
				;

			// Hidden options, will be allowed both on command line and
			// in config file, but will not be shown to the user.
			po::options_description hidden("Hidden options");
			hidden.add_options()
				//~ ("input-file", po::value< vector<string> >(), "input file")
				("input-file", po::value<string>(), "input file")
				;    
		
        // These two lines say that all positional options should be translated into "input-file" options. 
        po::positional_options_description p;
		p.add("input-file", -1);
		
        // Declare an options description instance which will include all the options
		po::options_description all("Allowed options");
        all.add(generic).add(config).add(hidden);

        // Declare an options description instance which will be shown to the user
        po::options_description visible("Allowed options");
        visible.add(generic).add(config);
		
		po::variables_map vm;
        store(po::command_line_parser(ac, av).options(all).positional(p).run(), vm);
		// no idea why, but this line let optimisation value be set to 10 and not to any random value
		po::notify(vm);
    
        if (vm.count("version")) {
            cout << "2.0" << "\n";
            return 0;
        }
              
        if (vm.count("help")) {
            //~ cout << visible;
            cout << all;
            return 0;
        }
              
		if (vm.count("include-path"))
		{
			cout << "Include paths are: " 
				 //~ << vm["include-path"].as<vector<string >>() << "\n";
				 << vm["include-path"].as<string>() << "\n";
		}

		if (vm.count("input-file"))
		{
			cout << "Input files are: " 
				 //~ << vm["input-file"].as<vector<string >>() << "\n";
				 << vm["input-file"].as<string>() << "\n";
		}

		cout << "Optimization level is " << opt << "\n";                
	
    }
    catch(exception& e) {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}
