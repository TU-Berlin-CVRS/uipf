#ifndef _CONVOLUTIONMODULE_
#define _CONVOLUTIONMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Applies Convolution filter to an image using openCV.
// See http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/filter_2d/filter_2d.html
class ConvolutionModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		ConvolutionModule(void): ModuleBase("convolution"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~ConvolutionModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //CONVOLUTIONMODULE
