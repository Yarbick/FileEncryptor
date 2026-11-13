#include "BaseAlgorithm.h"


FilePacket BaseAlgorithm::encrypt(const FilePacket &inputFilePacket) {
    clearOldData();
    prepareData(inputFilePacket);
    runEncrypt();

    return processResult();
}


FilePacket BaseAlgorithm::decrypt(const FilePacket &inputFilePacket) {
    clearOldData();
    prepareData(inputFilePacket);
    runDecrypt();

    return processResult();
}
