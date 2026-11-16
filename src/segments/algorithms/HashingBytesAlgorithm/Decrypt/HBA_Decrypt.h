#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/sharedFunctions/MathUtils/MathUtils.h"
#include "segments/shared/DataStructs.h"
#include "segments/shared/Constants.h"

#include <vector>
#include <cstdint>
#include <string>
#include <utility>


/**
 * @brief HashingBytesAlgorithm - Алгоритм для объединения байтов в 64-битные числа при помощи хеширования для последующей обработки другими алгоритмами. Расшифровывает
 */
class HBA_Decrypt final : public StaticAlgorithm<HBA_Decrypt> {
    friend class StaticAlgorithm<HBA_Decrypt>;

private:
    /// Основание хеша для возведения в степерь
    uint64_t hashBase = 0;
    /// Дополнительный коэффицент для умножения по модулю
    uint64_t subCoef = 0;
    /// Вектор с объединёнными хешированными байтами (блоками)
    std::vector<uint64_t> hashBlocks;
    /// Вектор с дехешированными байтами
    std::vector<uint8_t> rawBytes;

    HBA_Decrypt() = default;

protected:
    void clearOldData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
