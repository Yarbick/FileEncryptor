#include "FileOutConnector.h"


namespace FileOutConnector {
    namespace {
        /// Проверка файла на ошибки
        void checkFile(const std::ofstream &fout) {
            if (!fout.is_open()) {
                throw std::runtime_error("Failed to open file");
            }
        }


        /// Сборка тела файла
        void buildFileBody(std::ofstream &fout, const FilePacket &inFilePacket) {
            const auto bodySize = static_cast<std::streamsize>(inFilePacket.data.size());
            fout.write(reinterpret_cast<const char *>(inFilePacket.data.data()), bodySize);
        }


        /// Сборка зашифрованного файла
        void buildEncryptedFile(const std::string &filepath, const FilePacket &inFilePacket) {
            std::ofstream fout(filepath, std::ios::binary);
            checkFile(fout);

            // Запись заголовков
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

        /// Сборка расшифрованного файла
        void buildDecryptedFile(const std::string &filepath, const FilePacket &inFilePacket) {
            std::ofstream fout(filepath, std::ios::binary);
            checkFile(fout);

            buildFileBody(fout, inFilePacket);
        }
    }


    void transmitData(const std::string &dirpath, const FilePacket &inFilePacket, const Query &query) {
        // Сборка нужного пути к файлу и запуск передачи
        std::string filepath = dirpath + '/';
        if (query.mode == ModeType::ENCRYPT) {
            filepath += inFilePacket.params.at("filename") + '.' + ENCRYPTED_FILE_EXTENSION;
            buildEncryptedFile(filepath, inFilePacket);
        }
        if (query.mode == ModeType::DECRYPT) {
            filepath += inFilePacket.params.at("filename") + '.' + inFilePacket.params.at("extension");
            buildDecryptedFile(filepath, inFilePacket);
        }
    }
}
