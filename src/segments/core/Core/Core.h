#pragma once

#include "segments/core/connectors/FileInConnector/FileInConnector.h"
#include "segments/core/connectors/FileOutConnector/FileOutConnector.h"
#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/HashingBytesAlgorithm/Encrypt/HBA_Encrypt.h"
#include "segments/algorithms/HashingBytesAlgorithm/Decrypt/HBA_Decrypt.h"
#include "segments/algorithms/DiophantusAlgorithm/Encrypt/DA_Encrypt.h"
#include "segments/algorithms/DiophantusAlgorithm/Decrypt/DA_Decrypt.h"
#include "segments/algorithms/TurtleAlgorithm/Encrypt/TA_Encrypt.h"
#include "segments/algorithms/TurtleAlgorithm/Decrypt/TA_Decrypt.h"
#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"
#include "segments/shared/Constants.h"

#include <stdexcept>
#include <utility>
#include <string>


/**
 * @brief Главный класс, в котором объединяется вся логика и происходит взаимодействие между сегментами программы
 */
namespace Core {
    /**
     * @brief Обрабатывает запрос от пользователя. Получает данные из файла, выбирает и настраивает алгоритм, сохраняет получившийся результат в файл
     * @param inFilepath Путь к файлу для обработки
     * @param outDirpath Путь к папке, где сохранится результат
     * @param mainFilePacket Данные о запросе (параметры с полей)
     * @param query Настройки запроса
     * @return Статус ответа программы
     */
    ProcessResponse processQuery(const std::string &inFilepath, std::string &outDirpath, FilePacket &mainFilePacket,
                                 const Query &query);
}
