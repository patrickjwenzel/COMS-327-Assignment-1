class singleton {
private:
  static singleton *the_instance;
  singleton() {}
  singleton(const singleton &s) {}
  singleton &operator=(const singleton &s) { return *the_instance; }
public:
  static singleton *instance();
};

singleton *singleton::the_instance;

singleton *singleton::instance()
{
  if (!the_instance) {
    the_instance = new singleton;
  }

  return the_instance;
}

int main(int argc, char *argv[])
{
  singleton *s;

  s = singleton::instance();

}
