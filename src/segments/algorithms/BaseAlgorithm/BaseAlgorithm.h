#pragma once

#include "segments/shared/DataStructs.h"


class BaseAlgorithm {
public:
    virtual ~BaseAlgorithm() = default;

    virtual FilePacket encrypt(const FilePacket &inputFile);

    virtual FilePacket decrypt(const FilePacket &inputFile);

protected:
    virtual void clearOldData() = 0;

    virtual void prepareData(const FilePacket &inputFile) = 0;

    virtual void runEncrypt() = 0;

    virtual void runDecrypt() = 0;

    virtual FilePacket processResult() = 0;
};
