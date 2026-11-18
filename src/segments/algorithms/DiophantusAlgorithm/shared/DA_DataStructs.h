#pragma once

#include <cstdint>


/**
 * @brief Блок, состоящий из зашифрованных 2-ух байтов
 */
struct DA_Block {
    /// Коэфициент x из уравнения ax + by = c
    int32_t x;
    /// Коэфициент y из уравнения ax + by = c
    int32_t y;
    /// (LCM(A, B) / A) XOR multiplierCoef
    uint16_t Q;
    /// НОД двух шифруемых байтов
    uint16_t G;
};
