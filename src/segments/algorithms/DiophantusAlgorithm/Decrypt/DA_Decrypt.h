#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/DiophantusAlgorithm/shared/DA_DataStructs.h"
#include "segments/algorithms/DiophantusAlgorithm/shared/DA_Constants.h"
#include "segments/algorithms/sharedFunctions/MathUtils/MathUtils.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <random>
#include <string>
#include <utility>


/**
 * @brief DiophantusAlgorithm - Алгоритм шифрования, в основе которого лежит Диофантово уравнение (ax + by = c) с дополнительными операциями НОК и НОД. Расшифровывает
 */
class DA_Decrypt final : public StaticAlgorithm<DA_Decrypt> {
    friend class StaticAlgorithm<DA_Decrypt>;

private:
    /// Ключ для генерации коэффициентов для получения перменной C
    uint64_t seedKey = 0;
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<DA_Block> encryptedBlocks;
    /// Вектор с расшифрованными байтами
    std::vector<uint8_t> rawBytes;

    DA_Decrypt() = default;

protected:
    void clearOldData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
