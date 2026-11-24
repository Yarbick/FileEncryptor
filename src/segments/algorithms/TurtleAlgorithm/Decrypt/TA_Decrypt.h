#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/TurtleAlgorithm/shared/TA_Constants.h"
#include "segments/algorithms/TurtleAlgorithm/shared/TA_DataStructs.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <random>
#include <string>
#include <utility>


/**
 * @brief TurtleAlgorithm - Алгоритм шифрования, в основе которого лежит классическая задача на ДП - поиск оптимального маршрута из (0, 0) в (N, M) в матрице (NxM). Расшифровывает
 */
class TA_Decrypt final : public StaticAlgorithm<TA_Decrypt> {
    friend class StaticAlgorithm<TA_Decrypt>;

private:
    /// Ключ для генерации весов рёбер
    uint64_t seedKey = 0;
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<TA_Block> encryptedBlocks;
    /// Вектор с расшифрованными байтами
    std::vector<uint8_t> rawBytes;

    TA_Decrypt() = default;

protected:
    void clearOldData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
