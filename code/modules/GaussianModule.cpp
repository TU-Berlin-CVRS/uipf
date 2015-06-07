#include <iostream>
#include "GaussianModule.hpp"

using namespace std;
using namespace uipf;

// executes the Module
/*
input 		is a std::map of input resources, the names are described in the module meta description
params 		is a std::map of input paramaeters, the names are described in the module meta description
ouput 		is a std::map of output resources, the names are described in the module meta description
context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
*/
void GaussianModule::run(map<string, Data::ptr& >& input, map<string, string>& params, map<string, Data::ptr>& output ) const
{
	listParams(params);
	Matrix* oMatrix = getData<Matrix>(input,"image");

	// TODO implement actual gaussian

	cv::Mat m = oMatrix->getContent().clone();
	output.insert (pair < string, Data::ptr >("image", Matrix::ptr(new Matrix(m))));
}

std::string GaussianModule::name() const
{
	return "gaussian";
}

MetaData GaussianModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"windowSize", ParamDescription("window size of the kernel.") },
		{"sigma", ParamDescription("variance of the gaussian kernel.") }
	};

	return MetaData(
		"Applies Gaussian blurring to an image.",
		input,
		output,
		params
	);
}

