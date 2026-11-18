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


        /// Объединение двух ключей в один
        void mergeSubKeys(ProcessResponse &processResponse) {
            processResponse.params["mainKey"] =
                    processResponse.params["hashKey"] + '%' + processResponse.params["algorithmKey"];
            processResponse.params.erase("hashKey");
            processResponse.params.erase("algorithmKey");
        }

        /// Выделение двух ключей из одного
        void cutMainKey(FilePacket &mainFilePacket) {
            const uint64_t separatorIdx = mainFilePacket.params["mainKey"].find('%');
            if (separatorIdx == std::string::npos)
                throw std::runtime_error("Invalid mainKey format");

            mainFilePacket.params["hashKey"] = mainFilePacket.params["mainKey"].substr(0, separatorIdx);
            mainFilePacket.params["algorithmKey"] = mainFilePacket.params["mainKey"].substr(separatorIdx + 1);
            mainFilePacket.params.erase("mainKey");
        }


        /// Выбор алгоритма по параметрам запроса
        BaseAlgorithm &getAlgorithm(const Query &query) {
            switch (query.algorithm) {
                case AlgorithmType::HBA:
                    if (query.mode == ModeType::ENCRYPT) return HBA_Encrypt::instance();
                    return HBA_Decrypt::instance();
                case AlgorithmType::DA:
                    if (query.mode == ModeType::ENCRYPT) return DA_Encrypt::instance();
                    return DA_Decrypt::instance();
                default:
                    throw std::runtime_error("Unknown algorithm");
            }
        }
    }


    ProcessResponse processQuery(const std::string &inFilepath, std::string &outDirpath, FilePacket &mainFilePacket,
                                 const Query &query) {
        try {
            ProcessResponse processResponse;

            // Обработка входных данных
            FileInConnector::transmitData(inFilepath, mainFilePacket, query);
            preparePaths(inFilepath, outDirpath, mainFilePacket, query);

            // Выбор алгоритмов
            BaseAlgorithm &main_Algorithm = getAlgorithm(query);
            BaseAlgorithm &HBA_Algorithm = getAlgorithm({AlgorithmType::HBA, query.mode});

            if (query.mode == ModeType::ENCRYPT) {
                // Общая шифровка через хеширование байтов
                HBA_Algorithm.process(mainFilePacket, processResponse);
                // Основная шифровка через выбранный пользователем алгоритм
                main_Algorithm.process(mainFilePacket, processResponse);

                // Связывание двух ключей в один
                mergeSubKeys(processResponse);
            } else if (query.mode == ModeType::DECRYPT) {
                // Выделение двух ключей в из одного
                cutMainKey(mainFilePacket);

                // Основная расшифровка через выбранный пользователем алгоритм
                main_Algorithm.process(mainFilePacket, processResponse);
                // Общая расшифровка через дехеширование байтов
                HBA_Algorithm.process(mainFilePacket, processResponse);
            }

            // Сохранение результата в файл
            FileOutConnector::transmitData(outDirpath, mainFilePacket, query);

            processResponse.isSuccess = true;
            processResponse.message = "OK";
            return processResponse;
        } catch (const std::exception &error) {
            return ProcessResponse(false, error.what(), {});
        }
    }
}
