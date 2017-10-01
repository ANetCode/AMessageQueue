#ifndef AMQ_DEBUG_UTILS_H
#define AMQ_DEBUG_UTILS_H

#include <vector>
#include <string>

namespace amq {
    class debug{
    public:
        static std::vector<std::string> GetStackTrace();
        static void PrintfStackTrace();
    };
}

#endif // AMQ_DEBUG_UTILS_H
