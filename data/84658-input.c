#include "stdbit_stub.h"

#include "include/llvm-libc-macros/stdbit-macros.h"

#undef stdc_bit_width
#define stdc_bit_width(x)                                                      \
  _Generic((x),                                                                \
      unsigned char: stdc_bit_width_uc,                                       \
      unsigned short: stdc_bit_width_us,                                      \
      unsigned: stdc_bit_width_ui,                                            \
      unsigned long: stdc_bit_width_ul,                                       \
      unsigned long long: stdc_bit_width_ull)(x)

#undef stdc_bit_floor
#define stdc_bit_floor(x)                                                      \
  _Generic((x),                                                                \
      unsigned char: stdc_bit_floor_uc,                                       \
      unsigned short: stdc_bit_floor_us,                                      \
      unsigned: stdc_bit_floor_ui,                                            \
      unsigned long: stdc_bit_floor_ul,                                       \
      unsigned long long: stdc_bit_floor_ull)(x)

#include <assert.h>

#define CHECK_FUNCTION(FUNC_NAME, VAL)                                         \
  do {                                                                         \
    assert(FUNC_NAME((unsigned char)0U) == VAL##AU);                           \
    assert(FUNC_NAME((unsigned short)0U) == VAL##BU);                          \
    assert(FUNC_NAME(0U) == VAL##CU);                                          \
    assert(FUNC_NAME(0UL) == VAL##DU);                                         \
    assert(FUNC_NAME(0ULL) == VAL##EU);                                        \
  } while (0)

int main(void) {
  CHECK_FUNCTION(stdc_leading_zeros, 0xA);
  CHECK_FUNCTION(stdc_leading_ones, 0xB);
  CHECK_FUNCTION(stdc_trailing_zeros, 0xC);
  CHECK_FUNCTION(stdc_trailing_ones, 0xD);
  CHECK_FUNCTION(stdc_first_leading_zero, 0xE);
  CHECK_FUNCTION(stdc_first_leading_one, 0xF);
  CHECK_FUNCTION(stdc_first_trailing_zero, 0x0);
  CHECK_FUNCTION(stdc_first_trailing_one, 0x1);
  CHECK_FUNCTION(stdc_count_zeros, 0x2);
  CHECK_FUNCTION(stdc_count_ones, 0x3);

  assert(!stdc_has_single_bit((unsigned char)1U));
  assert(!stdc_has_single_bit((unsigned short)1U));
  assert(!stdc_has_single_bit(1U));
  assert(!stdc_has_single_bit(1UL));
  assert(!stdc_has_single_bit(1ULL));

  CHECK_FUNCTION(stdc_bit_width, 0x4);
  CHECK_FUNCTION(stdc_bit_floor, 0x5);
  CHECK_FUNCTION(stdc_bit_ceil, 0x6);

  return 0;
}