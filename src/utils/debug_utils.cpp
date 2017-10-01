#include "debug_utils.h"

#define BACKTRACE
#ifdef BACKTRACE
#include <iostream>
#include <execinfo.h>
#include <cxxabi.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;
using namespace amq;

std::vector<std::string> debug::GetStackTrace() {
    const int size = 128;
    int callstack[size] = {};
    int  frames = backtrace((void**) callstack, size);
    char** strs = backtrace_symbols((void**) callstack, frames);
    std::vector<std::string> stackFrames;
    stackFrames.reserve(frames);

    for (int i = 0; i < frames; ++i) {
        char functionSymbol[1024] = {};
        char moduleName[1024] = {};
        int  offset = 0;
        char addr[48] = {};
        sscanf(strs[i], "%*s %s %s %s %*s %d", (char*)&moduleName, (char*)&addr, (char*)&functionSymbol, &offset);
        int   validCppName = 0;
        char* functionName = abi::__cxa_demangle(functionSymbol, NULL, 0, &validCppName);
        char stackFrame[4096] = {};
        if (validCppName == 0) { // success
            sprintf(stackFrame, "(%s)\t0x%s — %s + %d",
                    moduleName, addr, functionName, offset);
        } else {
            sprintf(stackFrame, "(%s)\t0x%s — %s + %d",
                    moduleName, addr, functionName, offset);
        }

        if (functionName) {
            free(functionName);
        }
        std::string frameStr(stackFrame);
        stackFrames.push_back(frameStr);
    }
    free(strs);
    return stackFrames;
}
#else
std::vector<std::string> debug::GetStackTrace() {
    std::vector<std::string> v;
    return v;
}
#endif

void debug::PrintfStackTrace() {
    std::vector<std::string> bt = GetStackTrace();
    for(int i = 0; i < bt.size(); i++) {
        printf("%s\n", bt[i].c_str());
    }
}
