#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  volatile const int i = 7;

  cout << i << endl;

  *(int *) &i = 5;

  cout << i << endl;

  return 0;
}
