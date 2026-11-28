#include "STA_Encrypt.h"


void STA_Encrypt::clearData() {
    seedKey = 0;
    fullSum = 0;
    rawBytesPtr = nullptr;
    std::vector<STA_Block>().swap(encryptedBlocks);
}


void STA_Encrypt::prepareData(const FilePacket &inFilePacket) {
    // Генерация ключа для весов рёбер
    seedKey = rnd();

    // Изменение размера хешированных блоков с учётом изменения типа данных
    const uint64_t newSize = (inFilePacket.data.size() + STA_Const::BLOCK_LENGTH - 1) / STA_Const::BLOCK_LENGTH;
    encryptedBlocks.resize(newSize > 0 ? newSize - 1 : 0);

    rawBytesPtr = &inFilePacket.data;
}


void STA_Encrypt::runProcess() {
    const auto &rawBytes = *rawBytesPtr;

    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(STA_Const::BLOCK_LENGTH);
    hashPowers[0] = 1;
    for (int i = 1; i < STA_Const::BLOCK_LENGTH; i++) hashPowers[i] = hashPowers[i - 1] * STA_Const::HASH_BASE;

    // Объединение байтов в блоки через хеширование
    std::vector<uint64_t> currLayer((rawBytes.size() + STA_Const::BLOCK_LENGTH - 1) / STA_Const::BLOCK_LENGTH);
    for (uint64_t i = 0; i < rawBytes.size(); i += STA_Const::BLOCK_LENGTH) {
        const uint64_t blockIdx = i / STA_Const::BLOCK_LENGTH;

        for (int power = STA_Const::BLOCK_LENGTH - 1; power >= 0; power--) {
            const uint64_t idx = blockIdx * STA_Const::BLOCK_LENGTH + (STA_Const::BLOCK_LENGTH - power - 1);
            if (idx >= rawBytes.size()) break;
            currLayer[blockIdx] += rawBytes[idx] * hashPowers[power];
        }
    }

    // Шифрование слоями ДО
    uint64_t blockIdx = 0;
    std::vector<uint64_t> prevLayer;
    std::mt19937_64 subSeedKeyRnd(seedKey);
    while (currLayer.size() > 1) {
        // "Сдвиг" слоёв (храним лишь 2 слоя, постоянно двигая текущий в предыдущий для оптимизации памяти)
        std::swap(currLayer, prevLayer);
        currLayer.resize((prevLayer.size() + 1) / 2);

        // Генерация ключа для слоя и инициализация генератор для рёбер
        std::mt19937_64 weightRnd(subSeedKeyRnd());

        for (uint64_t i = 0; i < currLayer.size(); i++) {
            // Частный случай: последний нечётный параметр просто переходит вверх
            if (i * 2 + 1 >= prevLayer.size()) {
                currLayer[i] = prevLayer[i * 2];
                break;
            }

            // Генерация весов
            const uint64_t weightLeft = (weightRnd() % STA_Const::MAX_EDGE_WEIGHT) + 1;
            const uint64_t weightRight = (weightRnd() % STA_Const::MAX_EDGE_WEIGHT) + 1;
            const uint64_t weightDiff = weightRnd() % STA_Const::MOD;

            // Подсчёт новой суммы и значений блока
            const uint64_t child1 = (prevLayer[i * 2] * weightLeft) % STA_Const::MOD;
            const uint64_t child2 = (prevLayer[i * 2 + 1] * weightRight) % STA_Const::MOD;
            currLayer[i] = (child1 + child2) % STA_Const::MOD;
            encryptedBlocks[blockIdx++].diff = static_cast<uint32_t>(
                (static_cast<int64_t>(child1) - static_cast<int64_t>(child2) + weightDiff + STA_Const::MOD) %
                STA_Const::MOD);
        }
    }

    /// Сумма всего ДО
    fullSum = currLayer[0];
}


void STA_Encrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    const auto &rawBytes = *rawBytesPtr;

    // Запись параметров
    processResponse.params["algorithmKey"] = std::to_string(seedKey);
    outFilePacket.params["hashedSizeBody"] = std::to_string(rawBytes.size());
    outFilePacket.params["fullSum_STA"] = std::to_string(fullSum);

    // Запись зашифрованных блоков с учётом деления на байты
    outFilePacket.data.resize(encryptedBlocks.size() * STA_Const::BLOCK_SIZE);
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        STA_Block value(encryptedBlocks[i]);

        int prevSize = 0, currSize = sizeof(uint32_t);
        for (int byteIdx = currSize - 1; byteIdx >= prevSize; byteIdx--) {
            outFilePacket.data[i * STA_Const::BLOCK_SIZE + byteIdx] = static_cast<uint8_t>(value.diff & 0xFF);
            value.diff >>= BITS_PER_BYTE;
        }
    }
}
