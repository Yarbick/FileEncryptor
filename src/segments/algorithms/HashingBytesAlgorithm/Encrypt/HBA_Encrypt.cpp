#include "HBA_Encrypt.h"


void HBA_Encrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void HBA_Encrypt::prepareData(const FilePacket &inFilePacket) {
    // Создание новых параметров случайным образом
    hashBase = rnd() % HBA_Const::MAX_HASH_BASE + HBA_Const::MIN_HASH_BASE;
    subCoef = rnd() % HBA_Const::MAX_SUB_COEF + HBA_Const::MIN_SUB_COEF;

    // Изменение размера хешированных блоков с учётом изменения типа данных
    const uint64_t newSize = (inFilePacket.data.size() + HBA_Const::BLOCK_LENGTH - 1) / HBA_Const::BLOCK_LENGTH;
    hashedBlocks.resize(newSize);

    rawBytesPtr = &inFilePacket.data;
}


void HBA_Encrypt::runProcess() {
    const auto &rawBytes = *rawBytesPtr;

    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(HBA_Const::BLOCK_LENGTH);
    hashPowers[0] = 1;
    for (int i = 1; i < HBA_Const::BLOCK_LENGTH; i++) hashPowers[i] = hashPowers[i - 1] * hashBase;

    // Хеширование байтов
    for (uint64_t i = 0; i < hashedBlocks.size(); i++) {
        // Простое хеширование
        hashedBlocks[i] = 0;
        for (int power = HBA_Const::BLOCK_LENGTH - 1; power >= 0; power--) {
            const uint64_t idx = i * HBA_Const::BLOCK_LENGTH + (HBA_Const::BLOCK_LENGTH - power - 1);
            if (idx < rawBytes.size()) hashedBlocks[i] += rawBytes[idx] * hashPowers[power];
        }

        // Дополнение умножением на доп. коэффициент и связью с прошлым блоком через XOR
        hashedBlocks[i] = static_cast<uint64_t>((static_cast<__uint128_t>(hashedBlocks[i]) * subCoef) %
                                                HBA_Const::HASH_MOD);
        if (i != 0) hashedBlocks[i] ^= hashedBlocks[i - 1];
    }
}


void HBA_Encrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    const auto &rawBytes = *rawBytesPtr;

    // Запись параметров
    processResponse.params["hashKey"] = std::to_string(hashBase) + '&' + std::to_string(subCoef);
    outFilePacket.params["originalBodySize"] = std::to_string(rawBytes.size());

    // Запись хешированных блоков с учётом разделения на байты
    outFilePacket.data.resize(hashedBlocks.size() * HBA_Const::BLOCK_SIZE);
    for (uint64_t i = 0; i < hashedBlocks.size(); i++) {
        uint64_t value = hashedBlocks[i];

        for (int byteIdx = HBA_Const::BLOCK_SIZE - 1; byteIdx >= 0; byteIdx--) {
            outFilePacket.data[i * HBA_Const::BLOCK_SIZE + byteIdx] = static_cast<uint8_t>(value & 0xFF);
            value >>= BITS_PER_BYTE;
        }
    }
}
