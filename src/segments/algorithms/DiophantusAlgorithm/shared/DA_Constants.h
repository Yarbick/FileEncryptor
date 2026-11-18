#pragma once

#include <cstdint>


/// Модуль для коэффициента-множителя
constexpr uint64_t MULTIPLIER_MOD = 65535;
/// Количество объединяемых байтов в блок
constexpr uint64_t BLOCK_SIZE = 2;
/// Сдвиг для избежания нулей
constexpr uint64_t SHIFT = 1;
