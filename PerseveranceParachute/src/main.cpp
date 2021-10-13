#include "MainWindow.h"
#include <QTranslator>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QStringList languages = QLocale().uiLanguages();
        QTranslator translator;
        if (!languages.isEmpty() && (translator.load(QLocale(), QLatin1String("PerseveranceParachute"),
                            QLatin1String("_"), QLatin1String(":/french"),
                            QLatin1String(".qm")))) {
            QCoreApplication::installTranslator(&translator);
        }
    MainWindow w;
    w.show();
    return a.exec();

}
