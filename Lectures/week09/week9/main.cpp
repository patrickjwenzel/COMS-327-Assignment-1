#include <iostream>

#include "string327.h"

using namespace std;

int main(int argc, char *argv[])
{
  string327 s;
  string327 t("Welcome back from break.");

  cout << s << endl;
  cout << t << endl;

  s = t;
  cout << s << endl;

  cout << (s + t) << endl;
  cout << s << endl;
  cout << t << endl;

  s += t;
  cout << s << endl;
  cout << t << endl;

  cin >> s;

  cout << s;

  return 0;
}
