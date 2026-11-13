#include "FileOutConnector.h"


namespace FileOutConnector {
    namespace {
        void checkFile(const std::ofstream &fout) {
            if (!fout.is_open()) {
                throw std::runtime_error("Failed to open file");
            }
        }


        void buildFileBody(std::ofstream &fout, const FilePacket &inFilePacket) {
            const auto bodySize = static_cast<std::streamsize>(inFilePacket.data.size());
            fout.write(reinterpret_cast<const char *>(inFilePacket.data.data()), bodySize);
        }


        void buildEncryptedFile(const std::string &filename, const FilePacket &inFilePacket) {
            std::ofstream fout(filename, std::ios::binary);
            checkFile(fout);

            fout.write(reinterpret_cast<const char *>(ContainerFormat::MAGIC_BYTES), ContainerFormat::MAGIC_BYTES_SIZE);
            fout.write(reinterpret_cast<const char *>(&ContainerFormat::VERSION), sizeof(ContainerFormat::VERSION));

            const uint32_t paramsSize = inFilePacket.params.size();
            fout.write(reinterpret_cast<const char *>(&paramsSize), sizeof(paramsSize));

            for (const auto &[key, value]: inFilePacket.params) {
                const uint32_t keySize = key.size();
                fout.write(reinterpret_cast<const char *>(&keySize), sizeof(keySize));
                fout.write(key.data(), keySize);

                const uint32_t valueSize = value.size();
                fout.write(reinterpret_cast<const char *>(&valueSize), sizeof(valueSize));
                fout.write(value.data(), valueSize);
            }

            buildFileBody(fout, inFilePacket);
        }

        void buildDecryptedFile(const std::string &filename, const FilePacket &inFilePacket) {
            std::ofstream fout(filename, std::ios::binary);
            checkFile(fout);

            buildFileBody(fout, inFilePacket);
        }
    }


    void transmitData(const std::string &dirname, const FilePacket &inFilePacket, const Query &query) {
        std::string filename = dirname + '/';
        if (query.mode == ModeType::ENCRYPT) {
            filename += inFilePacket.params.at("filename") + '.' + ENCRYPTED_FILE_EXTENSION;
            buildEncryptedFile(filename, inFilePacket);
        }
        if (query.mode == ModeType::DECRYPT) {
            filename += inFilePacket.params.at("filename") + '.' + inFilePacket.params.at("extension");
            buildDecryptedFile(filename, inFilePacket);
        }
    }
}
