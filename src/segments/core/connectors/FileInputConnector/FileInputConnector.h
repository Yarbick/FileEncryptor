#pragma once

#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"

#include <fstream>
#include <string>
#include <stdexcept>
#include <ios>
#include <cstdint>


namespace FileInputConnector {
    void transmitData(const std::string &filename, FilePacket &outFilePacket, const Query &query);
}
