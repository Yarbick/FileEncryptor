#include "FileInputConnector.h"


namespace FileInputConnector {
    namespace {
        void checkFile(std::ifstream &fin) {
            if (not fin.is_open())
                throw std::runtime_error("Failed to open file");

            fin.seekg(0, std::ios::end);
            const std::streamsize fileSize = fin.tellg();
            fin.seekg(0, std::ios::beg);

            if (fileSize <= 0) throw std::runtime_error("File is empty");
        }

        void checkEncryptedFile(std::ifstream &fin) {
            checkFile(fin);

            fin.seekg(0, std::ios::end);
            const size_t fileSize = fin.tellg();
            fin.seekg(0, std::ios::beg);

            if (fileSize < ContainerFormat::HEADER_BYTES_SIZE)
                throw std::runtime_error("File is empty");

            std::vector<char> magicBytes(ContainerFormat::MAGIC_BYTES_SIZE);
            fin.read(magicBytes.data(), ContainerFormat::MAGIC_BYTES_SIZE);

            for (size_t i = 0; i < ContainerFormat::MAGIC_BYTES_SIZE; i++) {
                if (magicBytes[i] != ContainerFormat::MAGIC_BYTES[i])
                    throw std::runtime_error("The file is not encrypted");
            }

            uint8_t version = 0;
            fin.read(reinterpret_cast<char *>(&version), sizeof(version));

            if (version != ContainerFormat::VERSION)
                throw std::runtime_error("The encryption version does not match the current one");
        }


        void readFileBody(std::ifstream &fin, FilePacket &outFilePacket) {
            const std::streampos startPos = fin.tellg();
            fin.seekg(0, std::ios::end);
            const std::streamsize bodySize = fin.tellg() - startPos;
            fin.seekg(startPos, std::ios::beg);

            outFilePacket.data.resize(static_cast<size_t>(bodySize));
            fin.read(reinterpret_cast<char *>(outFilePacket.data.data()), bodySize);
        }


        void loadUserFile(const std::string &filename, FilePacket &outFilePacket) {
            std::ifstream fin(filename, std::ios::binary);
            checkFile(fin);

            readFileBody(fin, outFilePacket);
        }

        void loadEncryptedFile(const std::string &filename, FilePacket &outFilePacket) {
            std::ifstream fin(filename, std::ios::binary);
            checkEncryptedFile(fin);

            uint32_t paramsSize = 0;
            fin.read(reinterpret_cast<char *>(&paramsSize), sizeof(paramsSize));

            for (uint32_t i = 0; i < paramsSize; i++) {
                uint32_t keySize = 0;
                fin.read(reinterpret_cast<char *>(&keySize), sizeof(keySize));
                std::string key(keySize, '\0');
                fin.read(key.data(), keySize);

                uint32_t valueSize = 0;
                fin.read(reinterpret_cast<char *>(&valueSize), sizeof(valueSize));
                std::string value(valueSize, '\0');
                fin.read(value.data(), valueSize);

                outFilePacket.params[key] = value;
            }

            readFileBody(fin, outFilePacket);
        }
    }


    void transmitData(const std::string &filename, FilePacket &outFilePacket, const Query &query) {
        if (query.mode == ModeType::ENCRYPT)
            loadUserFile(filename, outFilePacket);
        if (query.mode == ModeType::DECRYPT)
            loadEncryptedFile(filename, outFilePacket);
    }
}
