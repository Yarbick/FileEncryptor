#include "HBA_Encrypt.h"


void HBA_Encrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void HBA_Encrypt::prepareData(const FilePacket &inFilePacket) {
    // Создание новых параметров случайным образом
    hashBase = rnd() % MAX_HASH_BASE + MIN_HASH_BASE;
    subCoef = rnd() % MAX_SUB_COEF + MIN_SUB_COEF;

    // Изменение размера хешированных блоков с учётом изменения типа данных
    const uint64_t newSize = (inFilePacket.data.size() + BLOCK_SIZE - 1) / BLOCK_SIZE;
    hashedBlocks.resize(newSize);

    rawBytesPtr = &inFilePacket.data;
}


void HBA_Encrypt::runProcess() {
    const auto &rawBytes = *rawBytesPtr;

    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(BLOCK_SIZE);
    hashPowers[0] = 1;
    for (int i = 1; i < BLOCK_SIZE; i++) hashPowers[i] = hashPowers[i - 1] * hashBase;

    // Хеширование байтов
    for (uint64_t i = 0; i < hashedBlocks.size(); i++) {
        // Простое хеширование
        hashedBlocks[i] = 0;
        for (int power = BLOCK_SIZE - 1; power >= 0; power--) {
            const uint64_t idx = i * BLOCK_SIZE + (BLOCK_SIZE - power - 1);
            if (idx < rawBytes.size()) hashedBlocks[i] += rawBytes[idx] * hashPowers[power];
        }

        // Дополнение умножением на доп. коэффициент и связью с прошлым блоком через XOR
        hashedBlocks[i] = static_cast<uint64_t>((static_cast<__uint128_t>(hashedBlocks[i]) * subCoef) % HASH_MOD);
        if (i != 0) hashedBlocks[i] ^= hashedBlocks[i - 1];
    }
}


void HBA_Encrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    constexpr uint64_t blockSize = sizeof(uint64_t);
    const auto &rawBytes = *rawBytesPtr;

    // Запись параметров
    processResponse.params["hashKey"] = std::to_string(hashBase) + '&' + std::to_string(subCoef);
    outFilePacket.params["originalBodySize"] = std::to_string(rawBytes.size());

    // Запись хешированных блоков с учётом разделения на байты
    outFilePacket.data.resize(hashedBlocks.size() * blockSize);
    for (uint64_t i = 0; i < hashedBlocks.size(); i++) {
        uint64_t value = hashedBlocks[i];

        for (int byteIdx = blockSize - 1; byteIdx >= 0; byteIdx--) {
            outFilePacket.data[i * blockSize + byteIdx] = static_cast<uint8_t>(value & 0xFF);
            value >>= BYTE;
        }
    }
}
