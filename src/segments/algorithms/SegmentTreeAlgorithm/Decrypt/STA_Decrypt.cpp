#include "STA_Decrypt.h"


void STA_Decrypt::clearOldData() {
    // Ничего не очищается - всё будет перезаписано в процессе алгоритма
}


void STA_Decrypt::prepareData(const FilePacket &inFilePacket) {
    // Получение параметров для алгоритма
    seedKey = std::stoull(inFilePacket.params.at("algorithmKey"));
    fullSum = std::stoull(inFilePacket.params.at("fullSum_STA"));

    // Изменение размера зашифрованных блоков с учётом объединения байтов
    const uint64_t newSize = inFilePacket.data.size() / STA_Const::BLOCK_SIZE;
    encryptedBlocks.resize(newSize);

    const uint64_t hashedSizeBody = std::stoull(inFilePacket.params.at("hashedSizeBody"));
    rawBytes.resize(hashedSizeBody);

    // Сборка зашифрованных блоков
    for (uint64_t i = 0; i < encryptedBlocks.size(); i++) {
        encryptedBlocks[i] = {0};

        int prevSize = 0, currSize = sizeof(uint32_t);
        for (int byteIdx = prevSize; byteIdx < currSize; byteIdx++) {
            encryptedBlocks[i].diff = (encryptedBlocks[i].diff << BITS_PER_BYTE) | inFilePacket.data[
                                          i * STA_Const::BLOCK_SIZE + byteIdx];
        }
    }
}


void STA_Decrypt::runProcess() {
    // Подсчёт степеней для основания хеша
    std::vector<uint64_t> hashPowers(STA_Const::BLOCK_LENGTH);
    hashPowers[0] = 1;
    for (int i = 1; i < STA_Const::BLOCK_LENGTH; i++) hashPowers[i] = hashPowers[i - 1] * STA_Const::HASH_BASE;

    // Предподсчёт размеров дерева
    std::vector<uint64_t> layerSizes = {encryptedBlocks.size() + 1};
    while (layerSizes.back() != 1) layerSizes.push_back((layerSizes.back() + 1) / 2);

    // Предподсчёт ключей для весов рёбер
    std::mt19937_64 subSeedKeyRnd(seedKey);
    std::vector<uint64_t> subSeedKeys(layerSizes.size());
    for (uint64_t i = 0; i < layerSizes.size(); i++) subSeedKeys[i] = subSeedKeyRnd();

    // Расшифрование слоями ДО
    uint64_t blockIdx = encryptedBlocks.size(), layerIdx = layerSizes.size() - 1;
    std::vector<uint64_t> currLayer = {fullSum}, prevLayer;
    while (layerIdx > 0) {
        // "Сдвиг" слоёв (храним лишь 2 слоя, постоянно двигая текущий в предыдущий для оптимизации памяти)
        std::swap(currLayer, prevLayer);
        currLayer.resize(layerSizes[layerIdx - 1]);

        // Генерация ключа для слоя и инициализация генератор для рёбер
        std::mt19937_64 weightRnd(subSeedKeys[layerIdx - 1]);

        blockIdx -= currLayer.size() / 2;
        const uint64_t inversedTwo = MathUtils::Ferman(2, STA_Const::MOD); // Обратная по модулю двойка
        for (uint64_t i = 0; i < prevLayer.size(); i++) {
            // Частный случай: последний нечётный параметр просто переходит вверх
            if (i * 2 + 1 >= currLayer.size()) {
                currLayer.back() = prevLayer[i];
                break;
            }

            // Генерация весов
            const uint64_t weightLeft = (weightRnd() % STA_Const::MAX_EDGE_WEIGHT) + 1;
            const uint64_t weightRight = (weightRnd() % STA_Const::MAX_EDGE_WEIGHT) + 1;
            const uint64_t weightDiff = weightRnd() % STA_Const::MOD;

            // Вычисление разницы без коэффициента
            uint64_t diff = (static_cast<int64_t>(encryptedBlocks[blockIdx + i].diff) + STA_Const::MOD - static_cast<
                                 int64_t>(weightDiff)) % STA_Const::MOD;

            // Вычисление слагаемых
            const uint64_t child1 = (((prevLayer[i] + STA_Const::MOD + diff) % STA_Const::MOD) * inversedTwo) %
                                    STA_Const::MOD;
            const uint64_t child2 = (((prevLayer[i] + STA_Const::MOD - diff) % STA_Const::MOD) * inversedTwo) %
                                    STA_Const::MOD;

            currLayer[i * 2] = (child1 * MathUtils::Ferman(weightLeft, STA_Const::MOD)) % STA_Const::MOD;
            currLayer[i * 2 + 1] = (child2 * MathUtils::Ferman(weightRight, STA_Const::MOD)) % STA_Const::MOD;
        }

        layerIdx--;
    }

    // Дехеширование конечных блоков
    for (uint64_t i = 0; i < currLayer.size(); i++) {
        uint64_t sourceValue = currLayer[i];

        for (int power = STA_Const::BLOCK_LENGTH - 1; power >= 0; power--) {
            const uint64_t idx = i * STA_Const::BLOCK_LENGTH + (STA_Const::BLOCK_LENGTH - power - 1);
            if (idx >= rawBytes.size()) break;
            rawBytes[idx] = sourceValue / hashPowers[power];
            sourceValue %= hashPowers[power];
        }
    }
}


void STA_Decrypt::prepareResult(FilePacket &outFilePacket, ProcessResponse &processResponse) {
    // Запись дешифрованных байтов
    outFilePacket.data = std::move(rawBytes);
}
