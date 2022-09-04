#include <cstdio>
#include <cassert>

#ACCEPT_SIGN(v) v =='-'

#define PARSE_SIGNED_INT { \
                           \
};

int main() {
  auto fp = fopen("example.csv", "r");
  assert(fp != nullptr);

  char c;
  while (1) {
    c = fgetc(fp);
    if (feof(fp)) {
      break;
    }
    printf("%c", c);
  }

  fclose(fp);
}
