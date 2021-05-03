#include <iostream>
#include <cstdlib>

using namespace std;

class shape {
public:
  int color;
  virtual double area() = 0;
  virtual double perimeter() = 0;
  virtual ~shape() {}
  friend ostream &operator<<(ostream &o, const shape &s);
  virtual ostream &print(ostream &o) const = 0;
};

ostream &operator<<(ostream &o, const shape &s)
{
  return s.print(o);
}

class rectangle : public shape {
protected:
  double width, height;
public:
  double area()
  {
    return width * height;
  }
  double perimeter()
  {
    return 2 * (width + height);
  }
  rectangle() : width(1), height(2) //Initialization lists also initialize
  {                                 //instance variables
  }
  rectangle(double width, double height) : width(width), height(height)
  {
  }
  ostream &print(ostream &o) const
  {
    return o << "[" << width << "," << height << "]";
  }
};

class square : public rectangle {
private:
public:
  square() : rectangle(1, 1) //Initialization list explicitly calls super
  {                          //class constructor
  }
  square(double side) : rectangle(side, side)
  {
  }
  ostream &print(ostream &o) const
  {
    return o << "[" << width << "]";
  }
};

int main(int argc, char *argv[])
{
  square s;
  rectangle r(3, 5);
  int i;
  shape *sp = &s;
  shape &sr = r;
  shape *a[10];

  cout << s.area() << " " << s.perimeter() << " " << r << endl;

  cout << sp->area() << " " << sp->perimeter() << " "
       << sr.area() << " " << sr.perimeter() << endl;

  for (i = 0; i < 10; i++) {
    if (rand() % 2) {
      a[i] = new square(1 + rand() % 10);
    } else {
      a[i] = new rectangle(1 + rand() % 10, 1 + rand() % 10);
    }
  }

  for (i = 0; i < 10; i++) {
    cout << *a[i] << endl;
  }

  return 0;
}
