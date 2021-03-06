#ifndef _SELECTLISTITEMMODULE_
#define _SELECTLISTITEMMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Selects one item from a list of items.
class SelectListItemModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		SelectListItemModule(void): ModuleBase("selectListItem"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~SelectListItemModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //SelectListItemModule
