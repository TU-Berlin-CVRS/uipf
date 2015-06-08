#include "Utils.hpp"

namespace uipf
{
namespace utils
{

std::string toLower(const std::string& strToEat)
{
       std::string strCopy (strToEat);
       std::transform(strCopy.begin(), strCopy.end(), strCopy.begin(), ::tolower);
       return strCopy;
}

bool endswith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() &&
           toLower(str).compare(str.size() - suffix.size(), suffix.size(), toLower(suffix)) == 0;
}
}

}
