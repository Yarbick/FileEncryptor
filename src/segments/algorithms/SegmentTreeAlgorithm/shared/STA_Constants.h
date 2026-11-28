#pragma once

#include "STA_DataStructs.h"

#include <cstdint>


namespace STA_Const {
    /// Модуль для всех операций в ДО
    constexpr uint64_t MOD = (1ull << 31ull) - 1;
    /// Максимальны вес для ребра
    constexpr uint64_t MAX_EDGE_WEIGHT = MOD / 2;
    /// Количество объединяемых байтов в блок
    constexpr uint64_t BLOCK_LENGTH = 3;
    /// Размер защифрованного блока в байтах
    constexpr uint64_t BLOCK_SIZE = sizeof(STA_Block);
    /// Основание хеша для объединения байтов
    constexpr uint64_t HASH_BASE = 256;
}
