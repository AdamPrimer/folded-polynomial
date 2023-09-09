//
// Folded Polynomial N64 Optimization for Efficient Sin/Cos Calculations.
//
// From the Kaze Emanuar video on YouTube: https://www.youtube.com/watch?v=hffgNRfL1XY
//
// Technique developed by SilasLock on the Kaze Emanuar Discord.
//

#include <iostream>
#include <math.h>
#include <stdint.h>

struct floatx2 {
  float a;
  float b;
};

#define ONE 1

#define SECOND_ORDER_COEFFICIENT 0.0000000010911122665310369f
#define quasi_cos_2(x) (ONE - SECOND_ORDER_COEFFICIENT * x * x)

#define FOURTH_ORDER_COEFFICIENTS_1 -0.0000000011485057369884462f
#define FOURTH_ORDER_COEFFICIENTS_2 0.00000000000000000021380733869182293f
#define quasi_cos_4(x) (ONE + x * x * (FOURTH_ORDER_COEFFICIENTS_1 + FOURTH_ORDER_COEFFICIENTS_2 * x * x))

floatx2 sincos_2nd_order(int16_t int_angle) {
  // Equivalent to taking cos of |(int_angle / (2^15)) * PI|.
  int32_t shifter = (int_angle ^ (int_angle << 1)) & 0xC000;
  float x = (float)(((int_angle + shifter) << 17) >> 16);
  float cosx = quasi_cos_2(x);
  float sinx = sqrtf(ONE - cosx * cosx);

  if (shifter & 0x4000) {
    float temp = cosx;
    cosx = sinx;
    sinx = temp;
  }

  if (int_angle < 0) {
    sinx = -sinx;
  }

  if (shifter & 0x8000) {
    cosx = -cosx;
  }

  return {sinx, cosx};
}

floatx2 sincos_4th_order(int16_t int_angle) {
  // Equivalent to taking cos of |(int_angle / (2^15)) * PI|.
  int32_t shifter = (int_angle ^ (int_angle << 1)) & 0xC000;
  float x = (float)(((int_angle + shifter) << 17) >> 16);
  float cosx = quasi_cos_4(x);
  float sinx = sqrtf(ONE - cosx * cosx);

  if (shifter & 0x4000) {
    float temp = cosx;
    cosx = sinx;
    sinx = temp;
  }

  if (int_angle < 0) {
    sinx = -sinx;
  }

  if (shifter & 0x8000) {
    cosx = -cosx;
  }

  return {sinx, cosx};
}

int main() {
  floatx2 a, b;
  for (int16_t i = INT16_MIN; i <= INT16_MAX; i++) {
    a = sincos_2nd_order(i);
    b = sincos_4th_order(i);
    std::cout << i << " | 2nd Order: " << a.a << " " << a.b << " | 4th Order: " << b.a << " " << b.b << std::endl;
    if (i == INT16_MAX) break;
  }
  return 0;
}
