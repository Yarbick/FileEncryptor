#pragma once

#include <cstdint>


/**
 * @brief Блок, состоящий из зашифрованных 3-ёх байтов, объединённых в 32-бинтое число. Старший бит - направление
 */
struct TA_Block {
    /// Значение зашифрованного блока
    uint32_t value;

    /**
     * @brief Получение направления
     * @return Направление (0 - пришли сверху, 1 - пришли слева)
     */
    uint32_t getDirection() const {
        return value >> 31;
    }

    /**
     * @brief Изменение направления
     * @param direction Новое направление (0 - пришли сверху, 1 - пришли слева)
     */
    void setDirection(const uint32_t direction) {
        value = (direction << 31) | getAbs();
    }

    /**
     * @brief Получение абсолютного значения
     * @return Абсолютное значение
     */
    uint32_t getAbs() const {
        return (value << 1) >> 1;
    }

    /**
     * @brief Изменение абсолютного значения
     * @param abs Новое абсолютное значение
     */
    void setAbs(const uint32_t abs) {
        value = (getDirection() << 31) | ((abs << 1) >> 1);
    }
};
