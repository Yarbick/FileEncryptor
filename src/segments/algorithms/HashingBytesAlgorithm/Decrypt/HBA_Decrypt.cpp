#include "HBA_Decrypt.h"

#include <stdexcept>


void HBA_Decrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void HBA_Decrypt::prepareData(const FilePacket &inFilePacket) {
    // Получение параметров для алгоритма
    const std::string &hashKey = inFilePacket.params.at("hashKey");
    const uint64_t separatorIdx = hashKey.find('&');
    if (separatorIdx == std::string::npos)
        throw std::out_of_range("Invalid hashKey format");

    hashBase = std::stoull(hashKey.substr(0, separatorIdx));
    subCoef = std::stoull(hashKey.substr(separatorIdx + 1));

    // Изменение размера хешированных байтов с учётом изменения типа данных
    const uint64_t newSize = inFilePacket.data.size() / 8;
    hashBlocks.resize(newSize);

    const uint64_t originalBodySize = std::stoull(inFilePacket.params.at("originalBodySize"));
    rawBytes.resize(originalBodySize);

    // Объединение хешированных байтов в блоки
    for (uint64_t i = 0; i < newSize; i++) {
        hashBlocks[i] = 0;
        for (int byteIdx = 0; byteIdx <= 7; byteIdx++) {
            hashBlocks[i] = (hashBlocks[i] << 8) | inFilePacket.data[i * 8 + byteIdx];
        }
    }
}


void HBA_Decrypt::runProcess() {
    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(BLOCK_SIZE);
    hashPowers[0] = 1;
    for (int i = 1; i < BLOCK_SIZE; i++) hashPowers[i] = hashPowers[i - 1] * hashBase;

    // Инвертированный subCoef
    const uint64_t invSubCoef = MathUtils::Ferman(subCoef, HASH_MOD);

    // Дехеширование байтов
    for (uint64_t i = 0; i < hashBlocks.size(); i++) {
        uint64_t block = hashBlocks[i];

        // Дополнение умножением на доп. коэффициент и связью с прошлым блоком через XOR
        if (i != 0) block ^= hashBlocks[i - 1];
        block = static_cast<uint64_t>((static_cast<__uint128_t>(block) * invSubCoef) % HASH_MOD);

        // Простое дехеширование
        for (int power = BLOCK_SIZE - 1; power >= 0; power--) {
            const uint64_t idx = i * BLOCK_SIZE + (BLOCK_SIZE - power - 1);
            if (idx < rawBytes.size()) rawBytes[idx] = block / hashPowers[power];
            block %= hashPowers[power];
        }
    }
}


void HBA_Decrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    // Запись дехешированных байтов
    outFilePacket.data = std::move(rawBytes);
}
