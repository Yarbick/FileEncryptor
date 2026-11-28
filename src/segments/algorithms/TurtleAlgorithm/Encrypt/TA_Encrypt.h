#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/TurtleAlgorithm/shared/TA_Constants.h"
#include "segments/algorithms/TurtleAlgorithm/shared/TA_DataStructs.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <random>
#include <chrono>
#include <string>


/**
 * @brief TurtleAlgorithm - Алгоритм шифрования, в основе которого лежит классическая задача на ДП - поиск оптимального маршрута из (0, 0) в (N, M) в матрице (NxM). Зашифровывает
 */
class TA_Encrypt final : public StaticAlgorithm<TA_Encrypt> {
    friend class StaticAlgorithm<TA_Encrypt>;

private:
    /// Ключ для генерации весов рёбер
    uint64_t seedKey = 0;
    /// Вектор с байтами для шифрования
    const std::vector<uint8_t> *rawBytesPtr = nullptr;
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<TA_Block> encryptedBlocks;
    /// Генератор псевдослучайных чисел для ключа
    std::mt19937_64 rnd{static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count())};

    TA_Encrypt() = default;

protected:
    void clearData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
