#include <QApplication>
#include <QWidget>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("FyleEncryptor - Тест Qt6");
    window.resize(400, 300);
    window.show();

    return QApplication::exec();
}
