#ifndef _SPLITCHANNELMODULE_
#define _SPLITCHANNELMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Split channels of an image into multiple Matrices.
class SplitChannelModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		SplitChannelModule(void): ModuleBase("splitChannel"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~SplitChannelModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //SPLITCHANNELMODULE
