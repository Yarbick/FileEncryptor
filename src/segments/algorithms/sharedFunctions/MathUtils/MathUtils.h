#pragma once

#include <cstdint>


/**
 * @brief Объединение математических методов
 */
namespace MathUtils {
    /**
     * @brief Бинарное возведение в степень по модулю
     * @param base Основание
     * @param power Степень
     * @param mod Модуль
     * @return Число: (base ^ power) % MOD
     */
    uint64_t binPowMod(__uint128_t base, __uint128_t power, __uint128_t mod);

    /**
     * @brief Получение обратного по простому модулю числа
     * @param N Число, для которого нужно найти обратное
     * @param mod Простой модуль
     * @return Число: (N ^ (-1)) % MOD
     */
    uint64_t Ferman(uint64_t N, uint64_t mod);

    /**
     * @brief Расширенный алгоритм Евклида (для байтов)
     * @param a - коэффициент a (первый байт)
     * @param b - коэффициент b (второй байт)
     * @param x - коэффициент x
     * @param y - коэффициент y
     * @return Число: gcd(a, b)
     */
    uint16_t extendedGCD(uint16_t a, uint16_t b, int32_t &x, int32_t &y);
}
