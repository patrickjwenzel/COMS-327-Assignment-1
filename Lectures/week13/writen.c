#include <unistd.h>
#include <errno.h>

int writen(int fd, const void *vptr, size_t n)
{
  size_t nleft;
  int nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;

  while (nleft > 0) {
    if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
      if (nwritten < 0 && errno == EINTR) {
        nwritten = 0;
      } else {
        return (-1);
      }
    }

    nleft -= nwritten;
    ptr += nwritten;
  }

  return n;
}