#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/SegmentTreeAlgorithm/shared/STA_DataStructs.h"
#include "segments/algorithms/SegmentTreeAlgorithm/shared/STA_Constants.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <random>
#include <chrono>
#include <string>


/**
 * @brief SegmentTreeAlgorithm - Алгоритм шифрования, в основе которого лежит Дерево Отрезков. Зашифровывает
 */
class STA_Encrypt final : public StaticAlgorithm<STA_Encrypt> {
    friend class StaticAlgorithm<STA_Encrypt>;

private:
    /// Ключ для генерации других ключей
    uint64_t seedKey = 0;
    /// Сумма всего ДО
    uint64_t fullSum = 0;
    /// Вектор с байтами для шифрования
    const std::vector<uint8_t> *rawBytesPtr = nullptr;
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<STA_Block> encryptedBlocks;
    /// Генератор псевдослучайных чисел для ключей
    std::mt19937_64 rnd{static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count())};

    STA_Encrypt() = default;

protected:
    void clearData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
