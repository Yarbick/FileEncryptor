#pragma once

#include "segments/shared/DataStructs.h"


/**
 * @brief Класс-родитель для алгоритмов
 */
class BaseAlgorithm {
public:
    virtual ~BaseAlgorithm() = default;

    /**
     * @brief Шифрование данных
     * @param inFilePacket Файловый пакет для шифрования
     * @return Отшифрованный файловый пакет
     */
    virtual FilePacket encrypt(const FilePacket &inFilePacket);

    /**
     * @brief Расшифрование данных
     * @param inFilePacket Файловый пакет для расшифрования
     * @return Расшифрованный файловый пакет
     */
    virtual FilePacket decrypt(const FilePacket &inFilePacket);

protected:
    /**
     * @brief Очистка алгоритма от старых данных
     */
    virtual void clearOldData() = 0;

    /**
     * @brief Подготавливает данные из файлового пакета для алгоритма
     * @param inFilePacket Файловый пакет с данными
     */
    virtual void prepareData(const FilePacket &inFilePacket) = 0;

    /**
     * @brief Запускает процесс шифрования
     */
    virtual void runEncrypt() = 0;

    /**
     * @brief Запускает процесс расшифрования
     */
    virtual void runDecrypt() = 0;

    /**
     * @brief Упаковка ответа алгоритма в файловый пакет
     * @return Файловый пакет с ответом
     */
    virtual FilePacket processResult() = 0;
};
