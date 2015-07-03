#include "Utils.hpp"

#include <algorithm>

namespace uipf{
namespace utils{


// converts a string to lower case
/*
strToEat	string, which has to be converted
*/
std::string toLower(const std::string& strToEat){

       std::string strCopy (strToEat);
       std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(), ::tolower);
       return strCopy;
}

// checks whether a string ends with a suffix
/*
str			string, which has to be checked
suffix	 	suffix, for which is searched in the string
*/
bool endswith(const std::string& str, const std::string& suffix){

    return str.size() >= suffix.size() &&
           toLower(str).compare(str.size() - suffix.size(), suffix.size(), toLower(suffix)) == 0;
}


// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
/*
s	old name, which has to be renamed
*/
std::string rename(std::string s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, '.')) {
		tokens.push_back(token);
	}

	std::string end = tokens[tokens.size()-1];
	tokens.pop_back();
	std::string beforeEnd = tokens[tokens.size()-1];
	tokens.pop_back();
	beforeEnd = beforeEnd +"_result";
	tokens.push_back(beforeEnd);
	tokens.push_back(end);

	std::string newName;

	newName = tokens[0];
	for (unsigned int i=1; i<tokens.size(); i++){
		newName = newName +"." + tokens[i];
	}

	return newName;
}

// gets the second part of the string, which is divided by : , e.g. source:../ball.png -> ../ball.png
/*
s	complete string, or only the second part
*/
std::string secondPart(std::string s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, ':')) {
		if (!token.empty())
			tokens.push_back(token);
	}

	if(tokens.size() == 1){
		return tokens[0];
	} else{
		return tokens[1];
	}
}

// gets the first part of the string, which is divided by : , e.g. source:../ball.png -> source
/*
s	complete string
*/
std::string firstPart(std::string s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, ':')) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens[0];
}


} // namespace utils
} // namespace uipf
