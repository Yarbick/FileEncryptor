#include "segments/core/Core/Core.h"
#include "segments/shared/Types.h"
#include "segments/shared/DataStructs.h"

#include <QApplication>
#include <QFileDialog>
#include <iostream>
#include <filesystem>


// Временные костыли от ИИ для тестов
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const QString inQStr = QFileDialog::getOpenFileName(nullptr, "Выберите файл");
    const QString outQStr = QFileDialog::getExistingDirectory(nullptr, "Выберите папку");

    if (inQStr.isEmpty() || outQStr.isEmpty()) return 0;

    const std::filesystem::path inFilepath(inQStr.toStdWString());
    const std::filesystem::path outDirpath(outQStr.toStdWString());

    FilePacket mainFilePacket;
    Query query{};

    int alg = 1, mode = 0;
    std::cin >> alg >> mode;

    if (alg == 1) query.algorithm = AlgorithmType::DA;
    else if (alg == 2) query.algorithm = AlgorithmType::TA;
    else if (alg == 3) query.algorithm = AlgorithmType::STA;

    if (mode == 0) query.mode = ModeType::ENCRYPT;
    else query.mode = ModeType::DECRYPT;

    if (query.mode == ModeType::DECRYPT) {
        std::cin >> mainFilePacket.params["mainKey"];
    }

    const ProcessResponse response = Core::processQuery(inFilepath, outDirpath, mainFilePacket, query);
    std::cout << response.message << std::endl;

    if (response.isSuccess && query.mode == ModeType::ENCRYPT) {
        for (const auto &[key, value]: response.params) {
            std::cout << key << ' ' << value << std::endl;
        }
    }

    return 0;
}
