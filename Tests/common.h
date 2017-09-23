#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <iostream>
#include <unistd.h>
#include <amq.h>
#include <unistd.h>


void run_server(int argc, const char** argv);
void run_client(int argc, const char** argv);

#endif // TEST_COMMON_H