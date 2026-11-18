#pragma once

#include <cstdint>


/**
 * @brief Типы алгоритмов
 */
enum class AlgorithmType : uint8_t {
    HBA = 0,
    DA = 1
};


/**
 * @brief Режимы работы программы
 */
enum class ModeType : uint8_t {
    ENCRYPT = 0,
    DECRYPT = 1
};
