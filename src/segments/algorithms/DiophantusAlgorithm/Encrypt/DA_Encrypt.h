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
#include <chrono>
#include <string>


/**
 * @brief DiophantusAlgorithm - Алгоритм шифрования, в основе которого лежит Диофантово уравнение (ax + by = c) с дополнительными операциями НОК и НОД. Зашифровывает
 */
class DA_Encrypt final : public StaticAlgorithm<DA_Encrypt> {
    friend class StaticAlgorithm<DA_Encrypt>;

private:
    /// Ключ для генерации коэффициентов для получения перменной C
    uint64_t seedKey = 0;
    /// Вектор с байтами для шифрования
    const std::vector<uint8_t> *rawBytesPtr = nullptr;
    /// Вектор с зашифрованными байтами (блоками)
    std::vector<DA_Block> encryptedBlocks;
    /// Генератор псевдослучайных чисел для ключа
    std::mt19937_64 rnd{static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count())};

    DA_Encrypt() = default;

protected:
    void clearOldData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
