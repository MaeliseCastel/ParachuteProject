#include <QFile>
#include <QDataStream>
#include "ParametersWriter.h"

void ParametersWriter::save(QString fileName, int sector, int tracks,
                            QString userText, bool blanks, char defaultChar,
                            QColor color0BitBinary, QColor color1BitBinary,
                            QColor color0BitParachute, QColor color1BitParachute) {
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << sector;
    out << tracks;
    out << userText;
    out << blanks;
    out << QChar::fromLatin1(defaultChar);
    out << color0BitBinary;
    out << color1BitBinary;
    out << color0BitParachute;
    out << color1BitParachute;
    file.close();
}
