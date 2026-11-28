#pragma once

#include <cstdint>


namespace TA_Const {
    /// Модуль для суммирования
    constexpr uint64_t SUM_MOD = 1ull << 31;
    /// Количество объединяемых байтов в блок
    constexpr uint64_t BLOCK_LENGTH = 3;
    /// Размер защифрованного блока в байтах
    constexpr uint64_t BLOCK_SIZE = sizeof(uint32_t);
    /// Основание хеша для объединения байтов
    constexpr uint64_t HASH_BASE = 256;
    /// Количество столбцов в матрице
    constexpr uint64_t M = 1024;
}
