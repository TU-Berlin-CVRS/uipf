#include "../framework/ModuleInterface.hpp"

namespace uipf{


class DummyModule : ModuleInterface{

	public:
		// constructor
		DummyModule(void);
		// destructor
		~DummyModule(void){};

		// runs the module chain with the params
		void run(std::map<std::string, uipf::Data>& input, std::map<std::string, std::string> params, std::map<std::string, uipf::Data>& output, uipf::Context context);


};

}
