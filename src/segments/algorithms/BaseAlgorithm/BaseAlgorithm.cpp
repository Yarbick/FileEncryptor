#include "BaseAlgorithm.h"


void BaseAlgorithm::process(FilePacket &inFilePacket, ProcessResponse &processResponse) {
    try {
        prepareData(inFilePacket);
        runProcess();
        prepareResult(inFilePacket, processResponse);
    } catch (...) {
        clearData();
        throw;
    }
    clearData();
}
