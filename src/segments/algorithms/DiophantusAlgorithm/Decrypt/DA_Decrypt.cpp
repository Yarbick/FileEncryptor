#include "DA_Decrypt.h"

#include <stdexcept>


void DA_Decrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void DA_Decrypt::prepareData(const FilePacket &inFilePacket) {
    // Получение параметров для алгоритма
    seedKey = std::stoull(inFilePacket.params.at("algorithmKey"));

    // Изменение размера зашифрованных блоков с учётом объединения байтов
    const uint64_t newSize = inFilePacket.data.size() / DA_Const::BLOCK_SIZE;
    encryptedBlocks.resize(newSize);

    const uint64_t hashedSizeBody = std::stoull(inFilePacket.params.at("hashedSizeBody"));
    rawBytes.resize(hashedSizeBody);

    // Сборка зашифрованных блоков
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        encryptedBlocks[i] = {0, 0, 0, 0};

        int prevSize = 0, currSize = sizeof(int32_t);
        uint32_t ux = 0;
        for (int byteIdx = prevSize; byteIdx < currSize; byteIdx++) {
            ux = (ux << BYTE) | inFilePacket.data[i * DA_Const::BLOCK_SIZE + byteIdx];
        }
        encryptedBlocks[i].x = static_cast<int32_t>(ux);

        prevSize = currSize, currSize += sizeof(int32_t);
        uint32_t yx = 0;
        for (int byteIdx = prevSize; byteIdx < currSize; byteIdx++) {
            yx = (yx << BYTE) | inFilePacket.data[i * DA_Const::BLOCK_SIZE + byteIdx];
        }
        encryptedBlocks[i].y = static_cast<int32_t>(yx);

        prevSize = currSize, currSize += sizeof(uint16_t);
        for (int byteIdx = prevSize; byteIdx < currSize; byteIdx++) {
            encryptedBlocks[i].Q = (encryptedBlocks[i].Q << BYTE) | inFilePacket.data[
                                       i * DA_Const::BLOCK_SIZE + byteIdx];
        }

        prevSize = currSize, currSize += sizeof(uint16_t);
        for (int byteIdx = prevSize; byteIdx < currSize; byteIdx++) {
            encryptedBlocks[i].G = (encryptedBlocks[i].G << BYTE) | inFilePacket.data[
                                       i * DA_Const::BLOCK_SIZE + byteIdx];
        }
    }
}


void DA_Decrypt::runProcess() {
    // Дешифрование байтов блоками
    std::mt19937_64 seedRnd(seedKey); // Коэффициенты повторятся с теми, что были при шифровке
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        DA_Block block = encryptedBlocks[i];

        // Генерация коэффициента
        const uint16_t multiplierCoef = static_cast<uint16_t>(seedRnd() % DA_Const::MULTIPLIER_MOD) + DA_Const::SHIFT;

        // Обратное решение Диофантова уравнения
        block.x /= multiplierCoef;
        block.y /= multiplierCoef;

        if (block.x == 0)
            throw std::runtime_error("Invalid algorithmKey");

        // Восстановление байтов
        uint16_t b = (block.Q xor multiplierCoef) * block.G;
        uint16_t a = (static_cast<int32_t>(block.G) - static_cast<int32_t>(b) * block.y) / block.x;
        // Сдвиг байтов обратно
        a -= DA_Const::SHIFT;
        b -= DA_Const::SHIFT;

        // Запись расшифрованнных байтов
        rawBytes[i * 2] = a;
        if (i * 2 + 1 < rawBytes.size()) rawBytes[i * 2 + 1] = b;
    }
}


void DA_Decrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    // Запись дещифрованных байтов
    outFilePacket.data = std::move(rawBytes);
}
