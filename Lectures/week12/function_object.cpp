#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class function_object {
public:
  function_object()
  {
    cout << __PRETTY_FUNCTION__ << endl;
  }
  function_object(function_object &f)
  {
    cout << __PRETTY_FUNCTION__ << endl;
  }
  function_object operator()()
  {
    return *this;
  }
};

class greater_than {
private:
  int value;
public:
  greater_than(int i)
  {
    value = i;
  }
  bool operator()(int i)
  {
    return i > value;
  }
};

bool less_than_5(int i)
{
  return i < 5;
}

void print_int(int i)
{
  cout << i << " ";
}

int main(int argc, char *argv[])
{
  function_object foo;

  foo();

  greater_than gt42(42);

  if (gt42(23)) {
    cout << "greater" << endl;
  } else {
    cout << "less" << endl;
  }
  if (gt42(57)) {
    cout << "greater" << endl;
  } else {
    cout << "less" << endl;
  }

  vector<int> v;
  vector<int>::iterator vi;

  for (int i = 0; i < 100; i++) {
    v.push_back(i);
  }

  for (vi = v.begin(); vi != v.end(); vi++) {
    cout << *vi << " ";
  }
  cout << endl;

  for (vi = v.begin(); (vi = find_if(vi, v.end(), gt42)) != v.end(); vi++) {
    cout << *vi << " ";
  }
  cout << endl;

  greater_than gt91(91);
  for (vi = v.begin(); (vi = find_if(vi, v.end(), gt91)) != v.end(); vi++) {
    cout << *vi << " ";
  }
  cout << endl;

  for (vi = v.begin(); (vi = find_if(vi, v.end(), less_than_5)) != v.end(); vi++) {
    cout << *vi << " ";
  }
  cout << endl;

  for_each(v.begin(), v.end(), print_int);
  for (vi = v.begin(); (vi = find_if(vi, v.end(), less_than_5)) != v.end(); vi++) {
    cout << *vi << " ";
  }
  cout << endl;

  for_each(v.begin(), v.end(), print_int);

  return 0;
}
