class visitor {
public:
  //One visit method for each visitable type
  virtual void visit_type1(type1 *t);
  virtual void visit_type2(type2 *t);
  virtual void visit_type3(type3 *t);
  virtual void visit_type4(type4 *t);
  //...
protected:
  visitor();
};

class visitable {
public:
  //Usefuk for the accept method; would be an interface in Java
  virtual ~visitable();
  virtual void accept(visitor &) = 0;
protected:
  visitable();
};

//Types that are visited in the visitor class
class type1 : public visitable
{
public:
  type1();
  //Accept method from visitable
  virtual void accept(visitor &v) { v.visit_type1(this); }
};

class type2 : public visitable
{
public:
  type2();
  virtual void accept(visitor &v) { v.visit_type2(this); }
};

//...

class printing_visitor : public visitor {
  virtual void visit_type1(visitable *v) { cout << v << endl; }
  virtual void visit_type2(visitable *v) { cout << v << endl; }
  //...
};

int main(int argc, char *argv[])
{
  type1 t1;
  printing_visitor pv;

  t1.accept(&pv);

  vector<visitable *> v;
  // Initialize v with a bunch of stuff...

  for (i = 0; i < v.size(); i++) {
    v[i].accept(&pv);
  }

  return 0;
}

