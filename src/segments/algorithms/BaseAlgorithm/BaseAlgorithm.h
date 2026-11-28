#pragma once

#include "segments/shared/DataStructs.h"

#include <stdexcept>


/**
 * @brief Класс-родитель для алгоритмов
 */
class BaseAlgorithm {
public:
    virtual ~BaseAlgorithm() = default;

    /**
     * @brief Обработка данных по алгоритму. Перезапишет данные в тот же файловый пакет
     * @param inFilePacket Файловый пакет с данными для обработки
     * @param processResponse Статус ответа программы, куда запишутся секретные параметры
     */
    void process(FilePacket &inFilePacket, ProcessResponse &processResponse);

protected:
    BaseAlgorithm() = default;

    /**
     * @brief Очистка алгоритма от старых данных
     */
    virtual void clearData() = 0;

    /**
     * @brief Подготавливает данные из файлового пакета для алгоритма
     * @param inFilePacket Файловый пакет с данными
     */
    virtual void prepareData(const FilePacket &inFilePacket) = 0;

    /**
     * @brief Запускает алгоритм
     */
    virtual void runProcess() = 0;

    /**
     * @brief Упаковка ответа алгоритма в файловый пакет
     * @param outFilePacket Файловый пакет, в который запишется результат
     * @param processResponse Статус ответа программы, куда запишутся секретные параметры
     */
    virtual void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) = 0;
};


/**
 * @brief "Прослойка" для классов, позволяющая работать лишь с одним экземпляром
 * @tparam Child Алгоритм, который подключается к прослойке
 */
template<class Child>
class StaticAlgorithm : public BaseAlgorithm {
public:
    StaticAlgorithm(const StaticAlgorithm &) = delete;

    StaticAlgorithm &operator=(const StaticAlgorithm &) = delete;

    /**
     * @brief Получение экземпляра класса
     * @return Экземпляр класса
     */
    static Child &instance() {
        static Child object;
        return object;
    }

protected:
    StaticAlgorithm() = default;
};
