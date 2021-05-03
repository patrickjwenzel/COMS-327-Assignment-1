#include <iostream>

using namespace std;

static int foo; // Symbol is not exported from compilation unit; same as C

static int bar() // Same!  Can't be called from another file; same as C
{
  return 0;
}

int baz()
{
  static int num_calls = 0; // Also same as C; lifetime is lifetime of program
  num_calls++;

  return num_calls;
}

class static_example {
private:
  static int num_instances; // Class variable
public:
  static_example()
  {
    num_instances++;
  }
  static int get_num_instances() // Static method; can be called without an
  {                              // instance of the class
    return num_instances;
  }
};


int main(int argc, char *argv[])
{
  int i;
  cout << static_example::get_num_instances() << endl;

  static_example se;

  cout << static_example::get_num_instances() << endl;

  for (i = 0; i < 100; i++) {
    new static_example;
  }

  cout << static_example::get_num_instances() << endl;

  return 0;
}
