#include "log_utils.h"
#include "amq.h"
using namespace std;
using namespace amq;

Log::Log(std::string const& filename) {
    fileLog = false;
    if (filename != "") {
        stream_.open(filename, std::ofstream::out | std::ofstream::app);
        if (stream_.is_open()) {
            fileLog = true;
        }
    }
}
Log::~Log(){}

Log& Log::debug() {
    static Log l("");
    return l;
}