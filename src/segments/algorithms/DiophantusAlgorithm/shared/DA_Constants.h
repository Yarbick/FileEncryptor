#pragma once

#include "DA_DataStructs.h"

#include <cstdint>


namespace DA_Const {
    /// Модуль для коэффициента-множителя
    constexpr uint64_t MULTIPLIER_MOD = 65535;
    /// Количество объединяемых байтов в блок
    constexpr uint64_t BLOCK_LENGTH = 2;
    /// Размер защифрованного блока в байтах
    constexpr uint64_t BLOCK_SIZE = sizeof(DA_Block);
    /// Сдвиг для избежания нулей
    constexpr uint64_t SHIFT = 1;
}
