#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/SegmentTreeAlgorithm/shared/STA_DataStructs.h"
#include "segments/algorithms/SegmentTreeAlgorithm/shared/STA_Constants.h"
#include "segments/algorithms/sharedFunctions/MathUtils/MathUtils.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <random>
#include <string>


/**
 * @brief SegmentTreeAlgorithm - Алгоритм шифрования, в основе которого лежит Дерево Отрезков. Расшифровывает
 */
class STA_Decrypt final : public StaticAlgorithm<STA_Decrypt> {
    friend class StaticAlgorithm<STA_Decrypt>;

private:
    /// Ключ для генерации других ключей
    uint64_t seedKey = 0;
    /// Сумма всего ДО
    uint64_t fullSum = 0;
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<STA_Block> encryptedBlocks;
    /// Вектор с расшифрованными байтами
    std::vector<uint8_t> rawBytes;

    STA_Decrypt() = default;

protected:
    void clearData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
