#include <rex/ui/image_decode.h>
#include "touch_icon_samples.h"
#include <cassert>
#include <iostream>

int main() {
  int width = 0, height = 0;
  const auto small = rex::ui::DecodeImageRGBA(kSmallIcon, sizeof(kSmallIcon), width, height, 1024, 1024);
  assert(width == 2 && height == 1 && small.size() == kSmallRgbaBytes);
  assert(small[0] == 17 && small[1] == 34 && small[2] == 51 && small[3] == 255);
  assert(rex::ui::DecodeImageRGBA(kTooWideIcon, sizeof(kTooWideIcon), width, height, 1024, 1024).empty());
  assert(width == 0 && height == 0);
  assert(!rex::ui::DecodeImageRGBA(kTooWideIcon, sizeof(kTooWideIcon), width, height).empty());
  assert(width == 2048 && height == 1);
  assert(rex::ui::DecodeImageRGBA(kSmallIcon, 4, width, height, 1024, 1024).empty());
  assert(rex::ui::DecodeImageRGBA(kSmallIcon, kOversizedInputLength, width, height, 1024, 1024).empty());
  assert(rex::ui::DecodeImageRGBA(nullptr, 0, width, height, 1024, 1024).empty());
  std::cout << "PASS icons decode correctly; oversized dimensions and malformed or overflowing input are rejected\n";
}
