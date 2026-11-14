#include "Core.h"


namespace Core {
    namespace {
        /// Обработка пути и извлечение нужной информации из него
        void preparePaths(const std::string &inFilepath, std::string &outDirpath, FilePacket &inFilePacket,
                          const Query &query) {
            // Обрабатока пути к папке для сохранения
            if (outDirpath.empty())
                throw std::runtime_error("Invalid output dirpath");
            if (outDirpath.back() == '/' || outDirpath.back() == '\\')
                outDirpath.pop_back();

            // При расшифровке все данные уже находятся в зашифрованном файле
            if (query.mode == ModeType::DECRYPT) return;

            // Отделяем от пути к файлу с данными имя и расширение
            int extensionSeparatorIdx = -1, filenameSeparatorIdx = -1;
            for (int idx = static_cast<int>(inFilepath.size() - 1); idx >= 0; idx--) {
                if (inFilepath[idx] == '.' and extensionSeparatorIdx == -1)
                    extensionSeparatorIdx = idx;
                if (inFilepath[idx] == '/' || inFilepath[idx] == '\\') {
                    filenameSeparatorIdx = idx;
                    break;
                }
            }

            if (extensionSeparatorIdx == -1 or filenameSeparatorIdx == -1)
                throw std::runtime_error("Invalid input filepath");

            const int filenameLength = extensionSeparatorIdx - filenameSeparatorIdx - 1;

            inFilePacket.params["filename"] = inFilepath.substr(filenameSeparatorIdx + 1, filenameLength);
            inFilePacket.params["extension"] = inFilepath.substr(extensionSeparatorIdx + 1);
        }
    }


    ProcessResponse processQuery(const std::string &inFilepath, std::string &outDirpath, FilePacket &inFilePacket,
                                 const Query &query) {
        try {
            // Обработка входных данных
            FileInConnector::transmitData(inFilepath, inFilePacket, query);
            preparePaths(inFilepath, outDirpath, inFilePacket, query);

            // Выбор и запуск алгоритма
            FilePacket outFilePacket;
            // TODO: сделать выбор алгоритма

            // Сохранение результата в файл
            FileOutConnector::transmitData(outDirpath, outFilePacket, query);

            return ProcessResponse(true, "OK");
        } catch (const std::exception &error) {
            return ProcessResponse(false, error.what());
        }
    }
}
