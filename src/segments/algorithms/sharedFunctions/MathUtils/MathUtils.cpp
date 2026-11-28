#include "MathUtils.h"


uint64_t MathUtils::binPowMod(__uint128_t base, __uint128_t power, const __uint128_t mod) {
    __uint128_t result = 1;
    while (power) {
        if (power & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        power >>= 1;
    }

    return result;
}


uint64_t MathUtils::Ferman(const uint64_t N, const uint64_t mod) {
    return binPowMod(N, mod - 2, mod);
}


uint16_t MathUtils::extendedGCD(const uint16_t a, const uint16_t b, int32_t &x, int32_t &y) {
    if (b == 0) {
        x = 1;
        y = 0;

        return a;
    }

    int32_t x1, y1;
    const uint16_t G = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;

    return G;
}
