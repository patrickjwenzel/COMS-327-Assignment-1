#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
  void *handle;
  int (*printf)(const char *, ...);

  if (!(handle = dlopen("./libmyprintf.so", RTLD_LAZY))) {
    perror("dlopen");

    return -1;
  }

  if (!(printf = dlsym(handle, "myprintf"))) {
    perror("dlsym");

    return -1;
  }

  printf("Hello World!\n");

  dlclose(handle);

  return 0;
}
