#include <cstdio>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <chrono>
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

  void SkipWhitespaceSeparatorWhitespace() {
    while (isspace(c)) Next();
    if (c == separator) {
      ++line_value;
      do { Next(); } while (isspace(c));
    } else if (c == '\n') {
      ++line;
      line_value = 0;
      Next();
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

using ParseF = std::function<void()>;

int main() {
  auto fp = fopen("myFile0.csv", "r");
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
  fpos_t start_pos;
  fgetpos(fp, &start_pos);

  auto skip_whitespaces = [&parser]() { parser.SkipWhitespaceSeparatorWhitespace(); };
  std::vector<uint32_t> u32r(10000);
  std::vector<int32_t> i32r(10000);
  std::vector<float_t> fr(10000);

  std::vector<ParseF> parsers = {
      [&parser, &u32r]() {
        u32r.push_back(parser.ParseU32());
        parser.SkipWhitespaceSeparatorWhitespace();
      },
//      skip_whitespaces,
      [&parser, &i32r]() {
        i32r.push_back(parser.ParseI32());
        parser.SkipWhitespaceSeparatorWhitespace();
      },
//      skip_whitespaces,
      [&parser, &fr]() {
        fr.push_back(parser.ParseFloat());
        parser.SkipWhitespaceSeparatorWhitespace();
      },
//      skip_whitespaces,
  };

  fsetpos(fp, &start_pos);
  parser.Next();

  auto start = std::chrono::steady_clock::now();
  for (size_t i = 0; i < 10000; i++) {
    for (const auto& p : parsers) p();
//    for (auto it = parsers.begin(); it != parsers.end(); ++it) {
//      (*it)();
//    }
  }
  auto end = std::chrono::steady_clock::now();
  auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "elapsed time: " << elapsed_ms.count() << "μs\n";

  fsetpos(fp, &start_pos);
  parser.Next();

  u32r.clear();
  i32r.clear();
  fr.clear();
  start = std::chrono::steady_clock::now();
  for (size_t i = 0; i < 10000; i++) {
    u32r.push_back(parser.ParseI32());
    parser.SkipWhitespaceSeparatorWhitespace();
    i32r.push_back(parser.ParseU32());
    parser.SkipWhitespaceSeparatorWhitespace();
    fr.push_back(parser.ParseFloat());
    parser.SkipWhitespaceSeparatorWhitespace();
  }
  end = std::chrono::steady_clock::now();
  elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "elapsed time: " << elapsed_ms.count() << "μs\n";

  fsetpos(fp, &start_pos);
  parser.Next();

  u32r.clear();
  i32r.clear();
  fr.clear();

  std::tuple<std::vector<uint32_t>, std::vector<int32_t>, std::vector<float_t>> vals;
  std::vector<size_t> cfg = {2, 1, 3};
  start = std::chrono::steady_clock::now();
  for (size_t i = 0; i < 10000; i++) {
    size_t j = 0;
    for (auto v : cfg) {
      switch (v) {
        case 1:
          parser.ParseI32();
          parser.SkipWhitespaceSeparatorWhitespace();
          continue;
        case 2:
          parser.ParseU32();
          parser.SkipWhitespaceSeparatorWhitespace();
          continue;
        case 3:
          parser.ParseFloat();
          parser.SkipWhitespaceSeparatorWhitespace();
          continue;
      }
    }
  }
  end = std::chrono::steady_clock::now();
  elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "elapsed time: " << elapsed_ms.count() << "μs\n";

  std::vector<std::vector<std::string>> vals_str;
  fclose(fp);

  fp = fopen("myFile0.csv", "r");
  assert(fp != nullptr);

  line = 0;
  std::string value;
  char buffer[100];
  std::vector<std::string> buf_line;
  start = std::chrono::steady_clock::now();
  while (line < 10000) {
    c = fgetc(fp);
    if (feof(fp)) {
      break;
    } else if (c == ',') {
      buf_line.push_back(std::string());
      value = std::string();
    } else if (c == '\n') {
      vals_str.push_back(std::move(buf_line));
      buf_line = std::vector<std::string>();
      ++line;
    } else {
      value += c;
    }
//    printf("%c", c);
  }
  end = std::chrono::steady_clock::now();
  elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "elapsed time: " << elapsed_ms.count() << "μs\n";

  fclose(fp);
}
