#pragma once

#include <vector>
#include <map>
#include <string>
#include <cstdint>


struct FilePacket {
    std::map<std::string, std::string> params;
    std::vector<uint8_t> data;
};
