#pragma once

#include "segments/core/connectors/FileInConnector/FileInConnector.h"
#include "segments/core/connectors/FileOutConnector/FileOutConnector.h"
#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"
#include "segments/shared/Constants.h"

#include <stdexcept>

/**
 * @brief Главный класс, в котором объединяется вся логика и происходит взаимодействие между сегментами программы
 */
namespace Core {
    /**
     * @brief Обрабатывает запрос от пользователя. Получает данные из файла, выбирает и настраивает алгоритм, сохраняет получившийся результат в файл
     * @param inFilepath Путь к файлу для обработки
     * @param outDirpath Путь к папке, где сохранится результат
     * @param inFilePacket Первичные данные о запросе (параметры с полей)
     * @param query Настройки запроса
     * @return Статус ответа программы
     */
    ProcessResponse processQuery(const std::string &inFilepath, std::string &outDirpath, FilePacket &inFilePacket,
                                 const Query &query);
}
