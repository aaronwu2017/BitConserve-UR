#include <Arduino.h>

void memzero(void *const pnt, const size_t len) {
  volatile uint8_t *volatile pnt_ = (volatile uint8_t *volatile)pnt;
  size_t i = 0;
  while (i < len) {
    pnt_[i++] = 0;
  }
}
