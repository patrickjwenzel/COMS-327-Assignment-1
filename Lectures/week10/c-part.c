#include "header.h"

int main(int argc, char *argv[])
{
  ostream *o;

  cpp_function("Hello World");

  o = return_printer();
  print(o, "Hello World!");

  return 0;
}
