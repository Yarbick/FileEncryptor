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
