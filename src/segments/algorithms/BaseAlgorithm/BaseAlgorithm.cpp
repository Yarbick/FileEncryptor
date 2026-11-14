#include "BaseAlgorithm.h"


FilePacket BaseAlgorithm::encrypt(const FilePacket &inFilePacket) {
    clearOldData();
    prepareData(inFilePacket);
    runEncrypt();

    return processResult();
}


FilePacket BaseAlgorithm::decrypt(const FilePacket &inFilePacket) {
    clearOldData();
    prepareData(inFilePacket);
    runDecrypt();

    return processResult();
}
