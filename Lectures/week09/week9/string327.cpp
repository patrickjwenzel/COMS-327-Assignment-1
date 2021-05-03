#include <cstring>
#include <cstdlib>

#include "string327.h"

string327::string327()
{
  str = (char *) malloc(1);
  str[0] = '\0';
}

string327::string327(const char *s)
{
  str = strdup(s);
}

string327::~string327()
{
  free(str);
}

int string327::length() const
{
  return strlen(str);
}

const char *string327::c_str() const
{
  return str;
}

int string327::operator<(const string327 &s)
{
  return strcmp(str, s.str) < 0;
}

int string327::operator>(const string327 &s)
{
  return strcmp(str, s.str) > 0;
}

int string327::operator==(const string327 &s)
{
  return strcmp(str, s.str) == 0;
}

int string327::operator!=(const string327 &s)
{
  return strcmp(str, s.str) != 0;
}

int string327::operator<=(const string327 &s)
{
  return strcmp(str, s.str) <= 0;
}

int string327::operator>=(const string327 &s)
{
  return strcmp(str, s.str) >= 0;
}

string327 string327::operator+(const string327 &s)
{
  string327 out;

  free(out.str);
  out.str = (char *) malloc(strlen(str) + strlen(s.str) + 1);
  strcpy(out.str, str);
  strcat(out.str, s.str);

  return out;
}

string327 &string327::operator+=(const string327 &s)
{
  str = (char *) realloc(str, strlen(str) + strlen(s.str) + 1);
  strcat(str, s.str);

  return *this;
}

string327 &string327::operator=(const string327 &s)
{
  free(str);
  str = strdup(s.str);

  return *this;
}

string327 &string327::operator=(const char *s)
{
  free(str);
  str = strdup(s);

  return *this;
}

char &string327::operator[](const int i)
{
  return str[i];
}

std::istream &operator>>(std::istream &i, const string327 &s)
{
  //Broken when input is longer than s.length()
  //Also broken because it stops at white space!
  return i >> s.str;
}

std::ostream &operator<<(std::ostream &o, const string327 &s)
{
  return o << s.c_str();
}
