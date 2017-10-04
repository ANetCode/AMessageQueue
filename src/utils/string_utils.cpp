#include "amq.h"
using namespace amq;
bool amq::StringEquals(std::string a, std::string b) {
    return a == b;
}


bool StringStartsWith(const std::string content, const std::string prefix) {
    return content.substr(0, prefix.size()) == prefix;
}

bool amq::StringStartsWith(const std::string content, const std::string prefix) {
    return content.substr(0, prefix.size()) == prefix;
}
