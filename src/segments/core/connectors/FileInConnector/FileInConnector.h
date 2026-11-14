#pragma once

#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"

#include <fstream>
#include <string>
#include <stdexcept>
#include <ios>
#include <cstdint>


/**
 * @brief Коннектор, загружающий данные из файла в файловый пакет
 */
namespace FileInConnector {
    /**
     * @brief Запуск транспортировки данных
     * @param filepath Путь к файлу, из которого извекаются данные
     * @param outFilePacket Файловый пакет, в который сохранятся данные
     * @param query Настройки запроса
     */
    void transmitData(const std::string &filepath, FilePacket &outFilePacket, const Query &query);
}
