#ifndef MATRIX_H
#define MATRIX_H

#include "../StdIncl.hpp"
#include "Data.hpp"
#include <opencv2/opencv.hpp>

namespace uipf{

// Matrix which is a specification of Data
class Matrix : public Data {
	public:
		typedef SMARTPOINTER<Matrix> ptr;
		typedef const SMARTPOINTER<Matrix> c_ptr;

	public:
		// constructor
		Matrix(void){};
		Matrix(cv::Mat&);
		// destructor
		~Matrix(void){};

		// get content (returns a cloned version of Mat by default)
		// this is due to prevent overwriting accidentally
		cv::Mat getContent(bool bAutoClone = true) const;

		// sets the content of the matrix
		void setContent(cv::Mat&);

		// returns the data type of this data object: in this case: MATRIX
		Type getType() override;

	private:
		// content of the matrix
		cv::Mat matrix_;
};

}

#endif
