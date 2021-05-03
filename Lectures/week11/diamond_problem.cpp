#include <iostream>

using namespace std;

class A {
public:
  A() { cout << "Constructing an A" << endl; }
  A(const A &a) { cout << "Copying an A" << endl; }
  virtual ~A() { cout << "Destroying an A" << endl; }
  virtual void print() { cout << "A" << endl; }
};

class B : virtual public A {
public:
  B() { cout << "Constructing an B" << endl; }
  B(const B &b) { cout << "Copying an B" << endl; }
  virtual ~B() { cout << "Destroying an B" << endl; }
  virtual void print() { cout << "B" << endl; }
};

class C : virtual public A{
public:
  C() { cout << "Constructing an C" << endl; }
  C(const C &c) { cout << "Copying an C" << endl; }
  virtual ~C() { cout << "Destroying an C" << endl; }
  virtual void print() { cout << "C" << endl; }
};

class D : virtual public B, virtual public C{
public:
  D() { cout << "Constructing an D" << endl; }
  D(const D &d) { cout << "Copying an D" << endl; }
  virtual ~D() { cout << "Destroying an D" << endl; }
  virtual void print() { cout << "D" << endl; }
};

int main(int argc, char *argv[])
{
  /*
  A a;

  B b;

  C c;
  */
  D d;

  d.print();
  d.C::print();
  d.B::print();
  d.A::print();

  return 0;
}

