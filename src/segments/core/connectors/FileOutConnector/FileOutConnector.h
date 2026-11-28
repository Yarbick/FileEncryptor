#pragma once

#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"
#include "segments/shared/Constants.h"

#include <fstream>
#include <string>
#include <stdexcept>
#include <ios>
#include <cstdint>
#include <filesystem>


/**
 * @brief Коннектор, выгружающий данные (ответ алгоритма) в файл в выбранную папку
 */
namespace FileOutConnector {
    /**
     * @brief Запуск транспортировки данных
     * @param dirpath Путь к папке, в которую сохранится файл
     * @param inFilePacket Данные, которые сохранятся в файл
     * @param query Настройки запроса
     */
    void transmitData(const std::filesystem::path &dirpath, const FilePacket &inFilePacket, const Query &query);
}
