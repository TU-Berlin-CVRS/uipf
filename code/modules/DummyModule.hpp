#include <string>
#include "../framework/Elem.hpp"
#include "../framework/ModuleInterface.hpp"
#include "../framework/Context.hpp"
#include "../framework/Configuration.hpp"

using namespace std;

class DummyModule : ModuleInterface{

	public:
		// constructor
		// loads the module chain with the params from the configuration object
		DummyModule(Configuration);
		// destructor
		~DummyModule(void){};

		// runs the module chain with the params
		void run(map<string, Elem>& input, map<string, string> params, map<string, Elem>& output, Context context);


};

