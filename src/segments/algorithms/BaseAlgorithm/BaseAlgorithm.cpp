#include "BaseAlgorithm.h"


void BaseAlgorithm::process(FilePacket &inFilePacket, ProcessResponse &processResponse) {
    clearOldData();
    prepareData(inFilePacket);
    runProcess();
    prepareResult(inFilePacket, processResponse);
}
