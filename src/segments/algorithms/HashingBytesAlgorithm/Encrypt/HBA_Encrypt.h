#pragma once

#include "segments/algorithms/BaseAlgorithm/BaseAlgorithm.h"
#include "segments/algorithms/HashingBytesAlgorithm/shared/HBA_Constants.h"
#include "segments/shared/Constants.h"
#include "segments/shared/DataStructs.h"

#include <vector>
#include <cstdint>
#include <random>
#include <chrono>
#include <string>


/**
 * @brief HashingBytesAlgorithm - Алгоритм для объединения байтов в 64-битные числа при помощи хеширования для последующей обработки другими алгоритмами. Зашифровывает
 */
class HBA_Encrypt final : public StaticAlgorithm<HBA_Encrypt> {
    friend class StaticAlgorithm<HBA_Encrypt>;

private:
    /// Основание хеша для возведения в степерь
    uint64_t hashBase = 0;
    /// Дополнительный коэффицент для умножения по модулю
    uint64_t subCoef = 0;
    /// Вектор с байтами для хеширования
    const std::vector<uint8_t> *rawBytesPtr = nullptr;
    /// Вектор с объединёнными захешированными байтами (блоками)
    std::vector<uint64_t> hashedBlocks;
    /// Генератор псевдослучайных чисел для хешей
    std::mt19937_64 rnd{static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count())};

    HBA_Encrypt() = default;

protected:
    void clearOldData() override;

    void prepareData(const FilePacket &inFilePacket) override;

    void runProcess() override;

    void prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) override;
};
