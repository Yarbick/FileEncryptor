#include "segments/core/Core/Core.h"
#include "segments/shared/Types.h"
#include "segments/shared/DataStructs.h"

#include <iostream>
#include <string>


int main() {
    std::string inFilepath, outDirpath;
    std::getline(std::cin, inFilepath);
    std::getline(std::cin, outDirpath);

    FilePacket mainFilePacket;
    Query query{};

    int alg, mode;
    std::cin >> alg >> mode;

    if (alg == 1) query.algorithm = AlgorithmType::DA;

    if (mode == 0) query.mode = ModeType::ENCRYPT;
    else query.mode = ModeType::DECRYPT;

    if (query.mode == ModeType::DECRYPT) {
        std::cin >> mainFilePacket.params["hashKey"];
        std::cin >> mainFilePacket.params["algorithmKey"];
    }

    ProcessResponse response = Core::processQuery(inFilepath, outDirpath, mainFilePacket, query);
    std::cout << response.message << std::endl;

    if (query.mode == ModeType::ENCRYPT) {
        for (const auto &[key, value]: response.params) {
            std::cout << key << ' ' << value << std::endl;
        }
    }

    return 0;
}
