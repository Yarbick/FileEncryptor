#pragma once

#include <cstdint>


/**
 * @brief Блок, состоящий из зашифрованных 3-ух байтов
 */
struct STA_Block {
    /// Разница между двумя детьми
    uint32_t diff;
};
