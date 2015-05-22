#include "../framework/ModuleInterface.hpp"

using namespace std;

class DummyModule : ModuleInterface{

	public:
		// constructor
		DummyModule(void);
		// destructor
		~DummyModule(void){};

		// runs the module chain with the params
		void run(map<string, Elem>& input, map<string, string> params, map<string, Elem>& output, Context context);


};

