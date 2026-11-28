#include "HBA_Decrypt.h"


void HBA_Decrypt::clearData() {
    hashBase = 0;
    subCoef = 0;
    std::vector<HBA_Block>().swap(encryptedBlocks);
    std::vector<uint8_t>().swap(rawBytes);
}


void HBA_Decrypt::prepareData(const FilePacket &inFilePacket) {
    // Получение параметров для алгоритма
    const std::string &hashKey = inFilePacket.params.at("hashKey");
    const uint64_t separatorIdx = hashKey.find('&');
    if (separatorIdx == std::string::npos)
        throw std::out_of_range("Invalid hashKey format");

    hashBase = std::stoull(hashKey.substr(0, separatorIdx));
    subCoef = std::stoull(hashKey.substr(separatorIdx + 1));

    // Изменение размера зашифрованных байтов с учётом изменения типа данных
    const uint64_t newSize = inFilePacket.data.size() / HBA_Const::BLOCK_SIZE;
    encryptedBlocks.resize(newSize);

    const uint64_t originalBodySize = std::stoull(inFilePacket.params.at("originalBodySize"));
    rawBytes.resize(originalBodySize);

    for (uint64_t i = 0; i < newSize; i++) {
        encryptedBlocks[i].hash = 0;

        for (int byteIdx = 0; byteIdx < HBA_Const::BLOCK_SIZE; byteIdx++) {
            encryptedBlocks[i].hash = (encryptedBlocks[i].hash << BITS_PER_BYTE) | inFilePacket.data[
                                          i * HBA_Const::BLOCK_SIZE + byteIdx];
        }
    }
}


void HBA_Decrypt::runProcess() {
    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(HBA_Const::BLOCK_LENGTH);
    hashPowers[0] = 1;
    for (int i = 1; i < HBA_Const::BLOCK_LENGTH; i++) hashPowers[i] = hashPowers[i - 1] * hashBase;

    // Инвертированный subCoef
    const uint64_t invSubCoef = MathUtils::Ferman(subCoef, HBA_Const::HASH_MOD);

    // Расшифрование байтов
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        HBA_Block block = encryptedBlocks[i];

        // Дополнение умножением на дополнительный коэффициент и связью с прошлым блоком через XOR
        if (i != 0) block.hash ^= encryptedBlocks[i - 1].hash;
        block.hash = static_cast<uint64_t>((static_cast<__uint128_t>(block.hash) * invSubCoef) % HBA_Const::HASH_MOD);

        // Простое дехеширование
        for (int power = HBA_Const::BLOCK_LENGTH - 1; power >= 0; power--) {
            const uint64_t idx = i * HBA_Const::BLOCK_LENGTH + (HBA_Const::BLOCK_LENGTH - power - 1);
            if (idx < rawBytes.size()) rawBytes[idx] = block.hash / hashPowers[power];
            block.hash %= hashPowers[power];
        }
    }
}


void HBA_Decrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    // Запись расшифрованных байтов
    outFilePacket.data = std::move(rawBytes);
}
