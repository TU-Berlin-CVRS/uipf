#ifndef MATRIX_H
#define MATRIX_H

#include "Data.hpp"

namespace uipf{

// Matrix which is a specification of Data
class Matrix : public Data {
	public:
		typedef SMARTPOINTER<Matrix> ptr;
		typedef const SMARTPOINTER<Matrix> c_ptr;

	public:
		// constructor
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

} // namespace

#endif
