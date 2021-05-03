#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
  ifstream f("input_file.txt");
  int i;
  string s;

  i = f.get();

  cout << i << endl;
  cout << (char) i << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.get() << endl;
  f.get();

  cout << (char) f.peek() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.peek() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.peek() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.peek() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.peek() << endl;
  cout << (char) f.get() << endl;
  cout << (char) f.peek() << endl;
  cout << (char) f.get() << endl;
  f.get();

  getline(f, s);
  cout << s << endl;
  f.get(); //Consumes newline (blank line)
  getline(f, s);
  cout << s << endl;

  f >> s;
  cout << s << endl;
  f >> s;
  cout << s << endl;

  cout << f.get() << endl; //The newline
  cout << f.get() << endl; //EOF


  return 0;
}
