#pragma once

#include "Types.h"

#include <vector>
#include <map>
#include <string>
#include <cstdint>


namespace ContainerFormat {
    inline constexpr size_t HEADER_BYTES_SIZE = 12, MAGIC_BYTES_SIZE = 7;
    inline constexpr uint8_t MAGIC_BYTES[] = {'Y', 'A', 'R', 'B', 'I', 'C', 'K'};
    inline constexpr uint8_t VERSION = 0;
}


struct FilePacket {
    std::map<std::string, std::string> params;
    std::vector<uint8_t> data;
};


struct Query {
    AlgorithmType algorithm;
    ModeType mode;
};


struct ProcessResponse {
    bool isSuccess;
    std::string message;
};
