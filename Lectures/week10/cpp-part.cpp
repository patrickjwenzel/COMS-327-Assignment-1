#include <iostream>

#include "header.h"

using namespace std;

void cpp_function(const char *s)
{
  cout << s << endl;
}

ostream *return_printer()
{
  return &cout;
}

void print(ostream *o, const char *s)
{
  *o << s << endl;
}
