#ifndef _RESIZEMODULE_
#define _RESIZEMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Applies Resize to an image using openCV.
// See http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html#resize
class ResizeModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		ResizeModule(void): ModuleBase("resize"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~ResizeModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //_RESIZEMODULE_
