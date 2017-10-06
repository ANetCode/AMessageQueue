#include "log_utils.h"
#include "amq.h"

#include <chrono>
#include <ctime>
#include <iomanip> // std::put_time

using namespace std;
using namespace amq;

// Define colors for printing to terminal
static const char* COL_NORM    = "\x1B[0m";   // Normal
static const char* COL_FATAL   = "\x1B[31m";  // Red
static const char* COL_ERROR   = "\x1B[91m";  // Light Red
static const char* COL_INFO    = "\x1B[37m";  // White
static const char* COL_WARN    = "\x1B[33m";  // Yellow
static const char* COL_DEBUG   = "\x1B[94m";  // Light Blue
static const char* COL_VERBOSE = "\x1B[36m";  // Cyan
static const char* COL_LOGGER  = "\x1B[90m";  // Dark Grey
static const char* COL_TRACE   = "\x1B[95m";  // Light Magenta
static const char* COL_END     = "\033[0m";   // Color End

// Utils funcs
template<typename Clock, typename Duration>
std::ostream &operator<<(std::ostream &stream,
    const std::chrono::time_point<Clock, Duration> &time_point) {
    const time_t time = Clock::to_time_t(time_point);
#if __GNUC__ > 4 || \
    ((__GNUC__ == 4) && __GNUC_MINOR__ > 8 && __GNUC_REVISION__ > 1)
    // Maybe the put_time will be implemented later?
    struct tm tm;
    localtime_r(&time, &tm);
    return stream << std::put_time(&tm, "%c"); // Print standard date&time
#else
    char buffer[26];
    ctime_r(&time, buffer);
    buffer[24] = '\0';  // Removes the newline that is added
    return stream << buffer;
#endif
}

Log::Log(std::string const& filename) {
    fileLog = false;
    if (filename != "") {
        stream_.open(filename, std::ofstream::out | std::ofstream::app);
        if (stream_.is_open()) {
            fileLog = true;
        }
    }
    log_color = COL_LOGGER;
}
Log::~Log(){}

static Log g_Log("");

Log& Log::debug(std::string file, int line) {
    g_Log.log_color = COL_DEBUG;
    g_Log.prelog(file, line);
    return g_Log;
}

Log& Log::warn(std::string file, int line) {
    g_Log.log_color = COL_WARN;
    g_Log.prelog(file, line);
    return g_Log;
}
Log& Log::error(std::string file, int line) {
    g_Log.log_color = COL_ERROR;
    g_Log.prelog(file, line);
    return g_Log;
}

const char* Log::end_line() {
    g_Log.flush();
    return "";
}

void Log::prelog(std::string file, int line) {
    ss_mem << "\n\033[0m" << "[" << std::chrono::system_clock::now() << "]" <<
    file << ": " << line << log_color << " ";
    if (fileLog) {
        ss_mem << "\n" <<  "[" << std::chrono::system_clock::now() << "]" <<
        file << ": " << line << log_color << " ";
    }
}

void Log::flush() {
    cout << ss_mem.str() << std::flush;
    ss_mem.str("");
    if (fileLog) {
        stream_ << ss_file.str() << std::flush;
        ss_file.str("");
    }
}