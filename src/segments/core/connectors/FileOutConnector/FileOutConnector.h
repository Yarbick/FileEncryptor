#pragma once

#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"
#include "segments/shared/Constants.h"

#include <fstream>
#include <string>
#include <stdexcept>
#include <ios>
#include <cstdint>


namespace FileOutConnector {
    void transmitData(const FilePacket &inFilePacket, const Query &query);
}
