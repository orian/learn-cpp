#include <cstdio>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <iostream>
#include "fast_float/fast_float.h"

class Parser {
  size_t line = 0;
  size_t line_value = 0;
  FILE* file;
  unsigned char buffer[100];
  unsigned char c;
  const unsigned char separator = ',';
  const unsigned char decimal_separator = '.';

 public:
  Parser(FILE* f) : file(f) {}

  bool Next() {
    c = fgetc(file);
    return !feof(file);
  }

  int32_t ParseI32() {
    int32_t num = 0;
    bool neg = false;
    if (c == '-') {
      neg = true;
      Next();
    }
    for (size_t i = 0; i < 10 && isdigit(c); ++i) {
      num = 10 * num + (c - '0');
      Next();
    }
    if (neg) num = -num;
    return num;
  }

  uint32_t ParseU32() {
    uint32_t num = 0;
    for (size_t i = 0; i < 10 && isdigit(c); ++i) {
      num = 10 * num + (c - '0');
      Next();
    }
    return num;
  }

  float_t ParseFloat() {
    unsigned char* ptr = buffer;
    if (c == '-') {
      (*ptr++) = c;
      Next();
    }
    *ptr = c;
    size_t i = 0;
    do {
      (*++ptr) = fgetc(file);
      ++i;
    } while (i < 97 && isdigit(*ptr));
    if (*ptr == decimal_separator) {
      // continue

      do {
        (*++ptr) = fgetc(file);
        ++i;
      } while (i < 97 && isdigit(*ptr));
    }
    c = *ptr;
    float num;
    fast_float::from_chars(reinterpret_cast<const char*>(buffer), reinterpret_cast<const char*>(ptr), num);
    return num;
  }

  void SkipWhitespaceSparatorWhitespace() {
    while (isspace(c)) Next();
    if (c == separator) {
      ++line_value;
      do { Next(); } while (isspace(c));
    } else if (c == '\n') {
      ++line;
      line_value = 0;
    }
  }
};

#define ACCEPT_SEPARATOR(v) {}

#define ACCEPT_DIGIT(v) {}

#define ACCEPT_N_DIGITS(n, v) {}

#define ACCEPT_SIGN(v) { \
  if ((v)=='-') next(); \
}

#define PARSE_SIGNED_INT(n) { \
  ACCEPT_SIGN(v);             \
  ACCEPT_N_DIGITS(3);         \
};

#define PARSE_INT8 {   \
   PARSE_SIGNED_INT(8) \
};

int main() {
  auto fp = fopen("example.csv", "r");
  assert(fp != nullptr);

  Parser parser(fp);
  char c;
  size_t line = 0;
  while (line == 0) {
    c = fgetc(fp);
    if (feof(fp)) {
      break;
    } else if (c == '\n') {
      ++line;
    }
    printf("%c", c);
  }
  parser.Next();
  std::cout << parser.ParseI32() << std::endl;
  parser.SkipWhitespaceSparatorWhitespace();
  std::cout << parser.ParseU32() << std::endl;
  parser.SkipWhitespaceSparatorWhitespace();
  std::cout << parser.ParseFloat() << std::endl;

  fclose(fp);
}
