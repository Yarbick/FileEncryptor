#include "TA_Decrypt.h"


void TA_Decrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void TA_Decrypt::prepareData(const FilePacket &inFilePacket) {
    // Получение параметров для алгоритма
    seedKey = std::stoull(inFilePacket.params.at("algorithmKey"));

    // Изменение размера зашифрованных блоков с учётом объединения байтов
    const uint64_t newSize = inFilePacket.data.size() / TA_Const::BLOCK_SIZE;
    encryptedBlocks.resize(newSize);

    const uint64_t hashedSizeBody = std::stoull(inFilePacket.params.at("hashedSizeBody"));
    rawBytes.resize(hashedSizeBody);

    // Сборка зашифрованных блоков
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        encryptedBlocks[i] = {0};

        int prevSize = 0, currSize = sizeof(uint32_t);
        for (int byteIdx = prevSize; byteIdx < currSize; byteIdx++) {
            encryptedBlocks[i].value = (encryptedBlocks[i].value << BITS_PER_BYTE) | inFilePacket.data[
                                           i * TA_Const::BLOCK_SIZE + byteIdx];
        }
    }
}


void TA_Decrypt::runProcess() {
    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(TA_Const::BLOCK_LENGTH);
    hashPowers[0] = 1;
    for (int i = 1; i < TA_Const::BLOCK_LENGTH; i++) hashPowers[i] = hashPowers[i - 1] * TA_Const::HASH_BASE;

    // Дешифрование байтов блоками
    std::mt19937 seedRnd(seedKey); // Восстановление весов рёбер
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        TA_Block block = encryptedBlocks[i];

        // Поиск перехода в ДП (обработка идёт по обоим случаям, чтобы сохранить порядок генерации весов
        TA_Block bestValue(-1);
        // Верхний сосед в матрице
        if (static_cast<int64_t>(i) - static_cast<int64_t>(TA_Const::M) >= 0) {
            const uint64_t topNeighbour = encryptedBlocks[i - TA_Const::M].getAbs();
            const uint64_t edgeWeight = seedRnd() % TA_Const::SUM_MOD;

            const uint32_t newAbs = (topNeighbour + edgeWeight) % TA_Const::SUM_MOD;
            if (block.getDirection() == 0) {
                bestValue.setAbs(newAbs);
                bestValue.setDirection(0);
            }
        }
        // Левый сосед в матрице
        if (static_cast<int64_t>(i) - 1 >= 0 && i % TA_Const::M != 0) {
            const uint64_t leftNeighbour = encryptedBlocks[i - 1].getAbs();
            const uint64_t edgeWeight = seedRnd() % TA_Const::SUM_MOD;

            const uint32_t newAbs = (leftNeighbour + edgeWeight) % TA_Const::SUM_MOD;
            if (block.getDirection() == 1) {
                bestValue.setAbs(newAbs);
                bestValue.setDirection(1);
            }
        }
        // Исключение для начальной клетки
        if (i == 0) bestValue.value = 0;

        // Подсчёт изначального значения блока
        uint32_t sourceValue = (static_cast<int32_t>(block.getAbs()) - static_cast<int32_t>(bestValue.getAbs()) +
                                TA_Const::SUM_MOD) % TA_Const::SUM_MOD;

        // Дехеширование блока байтов
        for (int power = TA_Const::BLOCK_LENGTH - 1; power >= 0; power--) {
            const uint64_t idx = i * TA_Const::BLOCK_LENGTH + (TA_Const::BLOCK_LENGTH - power - 1);
            if (idx >= rawBytes.size()) break;
            rawBytes[idx] = sourceValue / hashPowers[power];
            sourceValue %= hashPowers[power];
        }
    }
}


void TA_Decrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    // Запись дешифрованных байтов
    outFilePacket.data = std::move(rawBytes);
}
