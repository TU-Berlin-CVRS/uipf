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
		// default constructor
		Matrix() {}
		// constructor
		Matrix(cv::Mat& mat): matrix_(mat) {}
		// copy constructor
		Matrix(const Matrix& mat): matrix_(mat.matrix_.clone()) {}
		// destructor
		~Matrix(void){};

		// get content (returns a cloned version of Mat by default)
		// this is due to prevent overwriting accidentally
		cv::Mat getContent(bool bAutoClone = true) const;

		// sets the content of the matrix
		void setContent(cv::Mat&);

		// returns the data type of this data object: in this case: MATRIX
		Type getType() const override;

	private:
		// content of the matrix
		cv::Mat matrix_;
};

} // namespace

#endif
