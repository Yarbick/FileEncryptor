#pragma once

#include "segments/shared/DataStructs.h"


class BaseAlgorithm {
public:
    virtual ~BaseAlgorithm() = default;

    virtual FilePacket encrypt(const FilePacket &inputFilePacket);

    virtual FilePacket decrypt(const FilePacket &inputFilePacket);

protected:
    virtual void clearOldData() = 0;

    virtual void prepareData(const FilePacket &inputFilePacket) = 0;

    virtual void runEncrypt() = 0;

    virtual void runDecrypt() = 0;

    virtual FilePacket processResult() = 0;
};
