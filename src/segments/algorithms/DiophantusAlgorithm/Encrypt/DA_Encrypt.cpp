#include "DA_Encrypt.h"


void DA_Encrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void DA_Encrypt::prepareData(const FilePacket &inFilePacket) {
    // Генерация ключа для коэффициентов
    seedKey = rnd();

    // Изменение размера зашифрованных блоков с учётом объединения байтов
    encryptedBlocks.resize((inFilePacket.data.size() + BLOCK_SIZE - 1) / BLOCK_SIZE);

    rawBytesPtr = &inFilePacket.data;
}


void DA_Encrypt::runProcess() {
    const auto &rawBytes = *rawBytesPtr;

    // Шифрование байтов блоками
    std::mt19937_64 seedRnd(seedKey); // Для каждого блока будет генерироваться отдельный коэффициент
    for (uint64_t i = 0; i < rawBytes.size(); i += 2) {
        // Двигаем биты на +1 влево для избежания деления на 0
        const uint16_t a = static_cast<uint16_t>(rawBytes[i]) + SHIFT;
        const uint16_t b = static_cast<uint16_t>(i + 1 < rawBytes.size() ? rawBytes[i + 1] : 0) + SHIFT;

        // Решение расширенного алгоритма Евклида
        int32_t x, y;
        const uint16_t G = MathUtils::extendedGCD(a, b, x, y);

        // Защита от деления на ноль
        if (x == 0) {
            x += b / G;
            y -= a / G;
        }

        // Генерация коэффициента
        const uint16_t multiplierCoef = static_cast<uint16_t>(seedRnd() % MULTIPLIER_MOD) + SHIFT;

        // Решение Диофантова уравнения
        x *= multiplierCoef;
        y *= multiplierCoef;
        const uint16_t Q = static_cast<uint16_t>(b / G) xor multiplierCoef;

        // Запись в блок
        encryptedBlocks[i / 2] = {x, y, Q, G};
    }
}


void DA_Encrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    constexpr uint64_t blockSize = sizeof(DA_Block);
    const auto &rawBytes = *rawBytesPtr;

    // Запись параметров
    processResponse.params["algorithmKey"] = std::to_string(seedKey);
    outFilePacket.params["hashedSizeBody"] = std::to_string(rawBytes.size());

    // Запись зашифрованных блоков с учётом деления на байты
    outFilePacket.data.resize(encryptedBlocks.size() * blockSize);
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        DA_Block value(encryptedBlocks[i]);

        int prevSize = 0, currSize = sizeof(int32_t);
        for (int byteIdx = currSize - 1; byteIdx >= prevSize; byteIdx--) {
            outFilePacket.data[i * blockSize + byteIdx] = static_cast<uint8_t>(value.x & 0xFF);
            value.x >>= BYTE;
        }

        prevSize = currSize, currSize += sizeof(int32_t);
        for (int byteIdx = currSize - 1; byteIdx >= prevSize; byteIdx--) {
            outFilePacket.data[i * blockSize + byteIdx] = static_cast<uint8_t>(value.y & 0xFF);
            value.y >>= BYTE;
        }

        prevSize = currSize, currSize += sizeof(uint16_t);
        for (int byteIdx = currSize - 1; byteIdx >= prevSize; byteIdx--) {
            outFilePacket.data[i * blockSize + byteIdx] = static_cast<uint8_t>(value.Q & 0xFF);
            value.Q >>= BYTE;
        }

        prevSize = currSize, currSize += sizeof(uint16_t);
        for (int byteIdx = currSize - 1; byteIdx >= prevSize; byteIdx--) {
            outFilePacket.data[i * blockSize + byteIdx] = static_cast<uint8_t>(value.G & 0xFF);
            value.G >>= BYTE;
        }
    }
}
