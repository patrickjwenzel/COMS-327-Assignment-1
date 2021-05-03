#include <stdio.h>
#include <stdarg.h>

__attribute__((constructor))
void init(void)
{
  printf("About to replace printf.\n");
}

__attribute__((destructor))
void destroy(void)
{
  printf("All done.\n");
}

int myprintf(const char *format, ...)
{
  int ret;
  va_list ap;

  printf("You are inside my nefarious printf!\n");

  va_start(ap, format);
  ret = vprintf(format, ap);
  va_end(ap);
  
  printf("Okay.  Now leaving.\n");

  return ret;
}
