#include "TA_Encrypt.h"


void TA_Encrypt::clearData() {
    seedKey = 0;
    rawBytesPtr = nullptr;
    std::vector<TA_Block>().swap(encryptedBlocks);
}


void TA_Encrypt::prepareData(const FilePacket &inFilePacket) {
    // Генерация ключа для весов рёбер
    seedKey = rnd();

    // Изменение размера хешированных блоков с учётом изменения типа данных
    const uint64_t newSize = (inFilePacket.data.size() + TA_Const::BLOCK_LENGTH - 1) / TA_Const::BLOCK_LENGTH;
    encryptedBlocks.resize(newSize);

    rawBytesPtr = &inFilePacket.data;
}


void TA_Encrypt::runProcess() {
    const auto &rawBytes = *rawBytesPtr;

    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(TA_Const::BLOCK_LENGTH);
    hashPowers[0] = 1;
    for (int i = 1; i < TA_Const::BLOCK_LENGTH; i++) hashPowers[i] = hashPowers[i - 1] * TA_Const::HASH_BASE;

    // Шифрование байтов блоками
    std::mt19937 seedRnd(seedKey); // Для каждого ребра между клетками будет генерироваться свой вес
    for (uint64_t i = 0; i < rawBytes.size(); i += TA_Const::BLOCK_LENGTH) {
        const uint64_t blockIdx = i / TA_Const::BLOCK_LENGTH;

        // Объединение байтов через хеширование
        encryptedBlocks[blockIdx].value = 0;
        for (int power = TA_Const::BLOCK_LENGTH - 1; power >= 0; power--) {
            const uint64_t idx = blockIdx * TA_Const::BLOCK_LENGTH + (TA_Const::BLOCK_LENGTH - power - 1);
            if (idx >= rawBytes.size()) break;
            encryptedBlocks[blockIdx].value += rawBytes[idx] * hashPowers[power];
        }

        // Поиск перехода в ДП
        TA_Block bestValue(-1);
        // Верхний сосед в матрице
        if (static_cast<int64_t>(blockIdx) - static_cast<int64_t>(TA_Const::M) >= 0) {
            const uint64_t topNeighbour = encryptedBlocks[blockIdx - TA_Const::M].getAbs();
            const uint64_t edgeWeight = seedRnd() % TA_Const::SUM_MOD;

            const uint32_t newAbs = (topNeighbour + edgeWeight) % TA_Const::SUM_MOD;
            if (bestValue.getAbs() > newAbs) {
                bestValue.setAbs(newAbs);
                bestValue.setDirection(0);
            }
        }
        // Левый сосед в матрице
        if (static_cast<int64_t>(blockIdx) - 1 >= 0 && blockIdx % TA_Const::M != 0) {
            const uint64_t leftNeighbour = encryptedBlocks[blockIdx - 1].getAbs();
            const uint64_t edgeWeight = seedRnd() % TA_Const::SUM_MOD;

            const uint32_t newAbs = (leftNeighbour + edgeWeight) % TA_Const::SUM_MOD;
            if (bestValue.getAbs() > newAbs) {
                bestValue.setAbs(newAbs);
                bestValue.setDirection(1);
            }
        }
        // Исключение для начальной клетки
        if (blockIdx == 0) bestValue.value = 0;

        // Подсчёт итового значения
        encryptedBlocks[blockIdx].setAbs((encryptedBlocks[blockIdx].getAbs() + bestValue.getAbs()) % TA_Const::SUM_MOD);
        encryptedBlocks[blockIdx].setDirection(bestValue.getDirection());
    }
}


void TA_Encrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    const auto &rawBytes = *rawBytesPtr;

    // Запись параметров
    processResponse.params["algorithmKey"] = std::to_string(seedKey);
    outFilePacket.params["hashedSizeBody"] = std::to_string(rawBytes.size());

    // Запись зашифрованных блоков с учётом деления на байты
    outFilePacket.data.resize(encryptedBlocks.size() * TA_Const::BLOCK_SIZE);
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        TA_Block value(encryptedBlocks[i]);

        int prevSize = 0, currSize = sizeof(uint32_t);
        for (int byteIdx = currSize - 1; byteIdx >= prevSize; byteIdx--) {
            outFilePacket.data[i * TA_Const::BLOCK_SIZE + byteIdx] = static_cast<uint8_t>(value.value & 0xFF);
            value.value >>= BITS_PER_BYTE;
        }
    }
}
