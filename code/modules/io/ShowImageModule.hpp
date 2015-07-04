#ifndef _SHOWIMAGEMODULE_
#define _SHOWIMAGEMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{


class ShowImageModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		ShowImageModule(void): ModuleBase("showImage"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~ShowImageModule(void){};

		// runs the module chain with the params
		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;
};

}

#endif //SHOWIMAGEMODULE
