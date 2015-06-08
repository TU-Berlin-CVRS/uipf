#ifndef UTILS_H
#define UTILS_H

#include "StdIncl.hpp" 

namespace uipf
{
namespace utils
{

std::string toLower(const std::string& strToEat);

bool endswith(const std::string& str, const std::string& suffix);

//convert anything to string (slow but working)
template<typename T>
std::string toString(T& value)
{
        std::stringstream ss;
        ss << value;
        return ss.str();
}

} //utils

} //uipf
#endif //STDINCL

