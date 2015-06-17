#ifndef MATRIX_H
#define MATRIX_H

#include "../StdIncl.hpp"
#include "Data.hpp"
#include <opencv2/opencv.hpp>

namespace uipf{

// Matrix which is a specification of Elem
class Matrix : public Data {
	public:
		typedef SMARTPOINTER<Matrix> ptr;
		typedef const SMARTPOINTER<Matrix> c_ptr;

	public:
		// constructor (can't be virtual!)
		Matrix(void){};
		
		Matrix(cv::Mat&);
		// destructor
		~Matrix(void){};

		// get content (returns a cloned version of Mat by default)
		// this is due to prevent overwriting accidentally
		cv::Mat getContent(bool bAutoClone = true) const;

		// set content
		void setContent(cv::Mat&);

		Type getType() override;

	private:
		// content
		cv::Mat matrix_;
};

}

#endif
