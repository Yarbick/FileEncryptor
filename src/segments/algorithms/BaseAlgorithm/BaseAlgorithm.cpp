#include "BaseAlgorithm.h"


FilePacket BaseAlgorithm::encrypt(const FilePacket &inputFile) {
    clearOldData();
    prepareData(inputFile);
    runEncrypt();

    return processResult();
}


FilePacket BaseAlgorithm::decrypt(const FilePacket &inputFile) {
    clearOldData();
    prepareData(inputFile);
    runDecrypt();

    return processResult();
}
