#pragma once

#include <cstdint>


/**
 * @brief Блок, состоящий из зашифрованных 4-ёх байтов
 */
struct HBA_Block {
    /// Хеш из 4-ёх байтов
    uint64_t hash;
};
