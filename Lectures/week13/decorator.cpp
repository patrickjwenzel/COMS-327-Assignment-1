#include <iostream>

using namespace std;

class shape {
public:
  virtual ~shape() {}
  virtual void draw() = 0;
};

class square : public shape {
public:
  void draw()
  {
    cout << "[]" << endl;
  }
};

class circle : public shape {
public:
  void draw()
  {
    cout << "()" << endl;
  }
};

class green_square : public square {
public:
  void draw()
  {
    cout << "green-[]" << endl;
  }
};

class shape_decorator : public shape {
private:
  shape *s;
public:
  shape_decorator(shape *s) : s(s) {}
  ~shape_decorator() { delete s; }
  void draw() { s->draw(); }
};

class green_shape : public shape_decorator {
public:
  green_shape(shape *s) : shape_decorator(s) {}
  void draw()
  {
    cout << "green-";
    shape_decorator::draw();
  }
};

class big_shape : public shape_decorator {
public:
  big_shape(shape *s) : shape_decorator(s) {}
  void draw()
  {
    cout << "BIG-";
    shape_decorator::draw();
  }
};

class cute_shape : public shape_decorator {
public:
  cute_shape(shape *s) : shape_decorator(s) {}
  void draw()
  {
    cout << "cute-";
    shape_decorator::draw();
  }
};

int main(int argc, char *argv[])
{
  square s;
  circle c;

  s.draw();
  c.draw();

  green_shape gs(new square);
  gs.draw();

  big_shape bs(new green_shape(new square));
  bs.draw();

  cute_shape cs(new green_shape(new big_shape(new big_shape(new cute_shape(new circle)))));
  cs.draw();

  return 0;
}
