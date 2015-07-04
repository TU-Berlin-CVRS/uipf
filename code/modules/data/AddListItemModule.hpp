#ifndef _ADDLISTITEMMODULE_
#define _ADDLISTITEMMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Create an empty list
class AddListItemModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		AddListItemModule(void): ModuleBase("addListItem"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~AddListItemModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //AddListItemModule
