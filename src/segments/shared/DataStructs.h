#pragma once

#include "Types.h"

#include <vector>
#include <map>
#include <string>
#include <cstdint>


/**
 * @brief Константы для управления заголовком расширения зашифрованного файла
 */
namespace ContainerFormat {
    /// Общая длина заголовка (магические байты + версия + размер параметров)
    inline constexpr size_t HEADER_BYTES_SIZE = 12;
    /// Длина магических байтов
    inline constexpr size_t MAGIC_BYTES_SIZE = 7;
    /// Магические байты
    inline constexpr uint8_t MAGIC_BYTES[] = {'Y', 'A', 'R', 'B', 'I', 'C', 'K'};
    /// Версия шифровки
    inline constexpr uint8_t VERSION = 0;
}


/**
 * @brief Файловый пакет, содержащий заголовочные параметры и тело файла
 */
struct FilePacket {
    /// Заголовочные параметры (параметры алгоритмов, имя и расширение файла...)
    std::map<std::string, std::string> params;
    /// Тело файла в байтах
    std::vector<uint8_t> data;
};


/**
 * @brief Запрос от пользователя
 */
struct Query {
    /// Тип алгоритма
    AlgorithmType algorithm;
    /// Режим работы (шифровка/расшифровка)
    ModeType mode;
};


/**
 * @brief Статус ответа программы
 */
struct ProcessResponse {
    /// Есть ответ или нет
    bool isSuccess;
    /// Сообщение об ошибке (или "OK", если её нет)
    std::string message;
};
