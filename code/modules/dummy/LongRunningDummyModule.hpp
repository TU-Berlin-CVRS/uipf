#ifndef _LONGRUNNIGDUMMYMODULE_
#define _LONGRUNNIGDUMMYMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{


class LongRunningDummyModule :  public QObject,ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		LongRunningDummyModule(void):ModuleBase("LongRunningDummyModule"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~LongRunningDummyModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;
};

}

#endif //_LONGRUNNIGDUMMYMODULE_
