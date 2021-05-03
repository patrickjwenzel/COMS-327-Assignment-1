#include <iostream>

using namespace std;

void cswap(int *x, int *y)
{
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

void cppswap(int &x, int &y)
{
  int tmp = x;
  x = y;
  y = tmp;
}

int main(int argc, char *argv[])
{
  int i;
  int &r = i;

  r = 0;

  cout << i << endl;

  i = 10;

  cout << r << endl;

  int a = 10, b = 20;

  cswap(&a, &b);

  cout << "a is " << a << ", and b is " << b << endl;

  cppswap(a, b);

  cout << "a is " << a << ", and b is " << b << endl;

  return 0;
}
