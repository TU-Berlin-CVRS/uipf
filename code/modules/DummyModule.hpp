#include "../framework/ModuleInterface.hpp"

using namespace std;

class DummyModule : ModuleInterface{

	public:
		// constructor
		DummyModule(void);
		// destructor
		~DummyModule(void){};

		// runs the module chain with the params
		void run(map<string, Data>& input, map<string, string> params, map<string, Data>& output, Context context);


};

