#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/sharedFunctions/MathUtils/MathUtils.h"
#include "segments/algorithms/HashingBytesAlgorithm/shared/HBA_Constants.h"
#include "segments/algorithms/HashingBytesAlgorithm/shared/HBA_DataStructs.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <string>
#include <utility>
#include <stdexcept>


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
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<HBA_Block> encryptedBlocks;
    /// Вектор с расшифрованными байтами
    std::vector<uint8_t> rawBytes;

    HBA_Decrypt() = default;

protected:
    void clearData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
