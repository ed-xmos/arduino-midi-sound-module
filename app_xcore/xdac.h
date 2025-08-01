#ifndef XDAC_H_
#define XDAC_H_
#include <stdio.h>

class Dac final {
  public:
  static void setup() {
  }

  static void set0to7(int16_t out) {
    printf("%d\n", out);
    // printf("set0to7: %d\n", out);

  }

  static void set8toF(int16_t out) {
    // printf("set8toF: %d\n", out);
  }
};

#endif