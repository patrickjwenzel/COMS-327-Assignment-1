#ifndef HEADER_H
# define HEADER_H

#ifdef __cplusplus
//This code is only seen by the C++ compiler
#include <iostream>
using namespace std;

extern "C" {
#else
//This code is only seen by the C compiler
typedef void ostream;
#endif

void cpp_function(const char *s);
ostream *return_printer();
void print(ostream *o, const char *s);

#ifdef __cplusplus
}
#endif

#endif
