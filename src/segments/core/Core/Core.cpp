#include "Core.h"


namespace Core {
    namespace {
        void preparePaths(const std::string &inputFilename, std::string &outputDirname, FilePacket &inputFilePacket,
                          const Query &query) {
            if (outputDirname.empty())
                throw std::runtime_error("Invalid output dirname");
            if (outputDirname.back() == '/' || outputDirname.back() == '\\')
                outputDirname.pop_back();

            if (query.mode == ModeType::DECRYPT) return;

            int extensionSeparatorIdx = -1, filenameSeparatorIdx = -1;
            for (int idx = static_cast<int>(inputFilename.size() - 1); idx >= 0; idx--) {
                if (inputFilename[idx] == '.' and extensionSeparatorIdx == -1)
                    extensionSeparatorIdx = idx;
                if (inputFilename[idx] == '/' || inputFilename[idx] == '\\') {
                    filenameSeparatorIdx = idx;
                    break;
                }
            }

            if (extensionSeparatorIdx == -1 or filenameSeparatorIdx == -1)
                throw std::runtime_error("Invalid input filename");

            const int filenameLength = extensionSeparatorIdx - filenameSeparatorIdx - 1;

            inputFilePacket.params["filename"] = inputFilename.substr(filenameSeparatorIdx + 1, filenameLength);
            inputFilePacket.params["extension"] = inputFilename.substr(extensionSeparatorIdx + 1);
        }
    }


    ProcessResponse processQuery(const std::string &inputFilename, std::string &outputDirname,
                                 FilePacket &inputFilePacket, const Query &query) {
        try {
            FileInputConnector::transmitData(inputFilename, inputFilePacket, query);
            preparePaths(inputFilename, outputDirname, inputFilePacket, query);

            FilePacket outputFilePacket;
            // TODO: сделать выбор алгоритма

            FileOutConnector::transmitData(outputDirname, outputFilePacket, query);

            return ProcessResponse(true, "OK");
        } catch (const std::exception &error) {
            return ProcessResponse(false, error.what());
        }
    }
}
