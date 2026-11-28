#include "Core.h"


namespace Core {
    namespace {
        /// Обработка пути и извлечение нужной информации из него
        void preparePaths(const std::filesystem::path &inFilepath, const std::filesystem::path &outDirpath,
                          FilePacket &inFilePacket, const Query &query) {
            // Обрабатока пути к папке для сохранения
            if (outDirpath.empty() || !std::filesystem::exists(outDirpath) || !
                std::filesystem::is_directory(outDirpath))
                throw std::runtime_error("Invalid output dirpath");

            // При расшифровке все данные уже находятся в зашифрованном файле
            if (query.mode == ModeType::DECRYPT) return;

            // Проверка пути с данными
            if (!std::filesystem::is_regular_file(inFilepath) || !inFilepath.has_stem())
                throw std::runtime_error("Invalid input filepath");

            // Получение данных из пути
            const auto stemU8 = inFilepath.stem().u8string();
            inFilePacket.params["filename"] = std::string(stemU8.begin(), stemU8.end());

            const auto extU8 = inFilepath.extension().u8string();
            inFilePacket.params["extension"] = extU8.size() > 1 ? std::string(extU8.begin() + 1, extU8.end()) : "";
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
                case AlgorithmType::TA:
                    if (query.mode == ModeType::ENCRYPT) return TA_Encrypt::instance();
                    return TA_Decrypt::instance();
                case AlgorithmType::STA:
                    if (query.mode == ModeType::ENCRYPT) return STA_Encrypt::instance();
                    return STA_Decrypt::instance();
                default:
                    throw std::runtime_error("Unknown algorithm");
            }
        }


        /// Вычисление хеша для проверки файлов
        uint64_t computeCheckSum(const std::vector<uint8_t> &data) {
            uint64_t hash = 14695981039346656037ull;
            for (const uint8_t byte: data) {
                hash ^= byte;
                hash *= 1099511628211ull;
            }

            return hash;
        }
    }


    ProcessResponse processQuery(const std::filesystem::path &inFilepath, const std::filesystem::path &outDirpath,
                                 FilePacket &mainFilePacket, const Query &query) {
        try {
            ProcessResponse processResponse;

            // Обработка входных данных
            FileInConnector::transmitData(inFilepath, mainFilePacket, query);
            preparePaths(inFilepath, outDirpath, mainFilePacket, query);

            // Выбор алгоритмов
            BaseAlgorithm &main_Algorithm = getAlgorithm(query);
            BaseAlgorithm &HBA_Algorithm = getAlgorithm({AlgorithmType::HBA, query.mode});

            if (query.mode == ModeType::ENCRYPT) {
                // Создание хеша для проверки на целостность файла
                mainFilePacket.params["checkSum"] = std::to_string(computeCheckSum(mainFilePacket.data));

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

                // Проверка на целостность файла
                if (!mainFilePacket.params.contains("checkSum") || mainFilePacket.params["checkSum"] != std::to_string(
                        computeCheckSum(mainFilePacket.data))) {
                    throw std::runtime_error("Invalid key or corrupted file");
                }
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
