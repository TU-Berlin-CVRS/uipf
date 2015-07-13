#ifndef Context_H
#define Context_H

#include "StdIncl.hpp"
#include "types/Matrix.hpp"

namespace uipf{


// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
class Context {
	public:
		// constructor
		Context() {};
		// destructor
		~Context(void){};

		// returns a flag indicating whether the processing chain is run from Command line (false) or from the GUI (true)
		bool hasGUI() { return bHasGUI_; };

		// returns a flag indicating whether the processing chain has been requested to stop. (the stop button has been pressed in the GUI).
		// Long running modules should check this flag periodically and exit if it is true.
		// If modules do not exit on their own , they get killed automatically after a grace period.
		bool isStopRequested() { return bStopRequested_; };

		// returns a flag indicating whether the processing chain has been paused. (normally due to a window that is shown in blocking mode).
		// This flag is usually checked by the module manager.
		bool isPaused() { return bStopRequested_; };

		// returns the current processing step name
		// can be useful to provide default settings for parameters like filenames and the like
		std::string getProcessingStepName() const { return processingStepName_; };


		// display an image in a window
		void displayImage(const std::string& strTitle, const Matrix& oMat, bool bBlocking, bool bAutoClose=false);

		// wait for user input
		void waitKey(std::string message = "");

	protected:

		// flag that communicates if the Application is run from Command line or with GUI
		bool bHasGUI_ = false;

		// if set, modules should abort their work (the stop button has been pressed in the GUI).
		volatile bool bStopRequested_ = false;

		// if set, the current execution is paused, waiting for user input to continue.
		volatile bool bPaused_ = false;

		// the current processing step name
		std::string processingStepName_;

		// allow ModuleManager to set properties of the context
		friend class ModuleManager;

};

} // namespace

#endif
