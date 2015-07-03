#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>

// This is a collection of help-methods

namespace uipf{
namespace utils{


// converts a string to lower case
std::string toLower(const std::string& strToEat);

// checks whether a string ends with a suffix
bool endswith(const std::string& str, const std::string& suffix);

//convert anything to string (slow but working)
template<typename T>
std::string toString(T& value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
}

// gets the first part of the string, which is divided by : , e.g. source:../ball.png -> source
std::string firstPart(std::string s);

// gets the second part of the string, which is divided by : , e.g. source:../ball.png -> ../ball.png
std::string secondPart(std::string s);

// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
std::string rename(std::string s);



} // namespace utils
} // namespace uipf

#endif
