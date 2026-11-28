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
        void buildEncryptedFile(const std::filesystem::path &filepath, const FilePacket &inFilePacket) {
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

            // Проверка на успешную запись
            if (!fout.good())
                throw std::runtime_error("Failed to write file");
        }

        /// Сборка расшифрованного файла
        void buildDecryptedFile(const std::filesystem::path &filepath, const FilePacket &inFilePacket) {
            std::ofstream fout(filepath, std::ios::binary);
            checkFile(fout);

            buildFileBody(fout, inFilePacket);

            // Проверка на успешную запись
            if (!fout.good())
                throw std::runtime_error("Failed to write file");
        }
    }


    void transmitData(const std::filesystem::path &dirpath, const FilePacket &inFilePacket, const Query &query) {
        // Сборка нужного пути к файлу и запуск передачи
        std::filesystem::path outPath(dirpath);
        std::string fullFilename = inFilePacket.params.at("filename");

        if (query.mode == ModeType::ENCRYPT) {
            fullFilename += '.' + ENCRYPTED_FILE_EXTENSION;
        }
        if (query.mode == ModeType::DECRYPT) {
            const std::string &extension = inFilePacket.params.at("extension");
            if (!extension.empty()) fullFilename += '.' + extension;
        }
        outPath /= std::filesystem::path(std::u8string(fullFilename.begin(), fullFilename.end()));

        if (query.mode == ModeType::ENCRYPT) buildEncryptedFile(outPath, inFilePacket);
        if (query.mode == ModeType::DECRYPT) buildDecryptedFile(outPath, inFilePacket);
    }
}
