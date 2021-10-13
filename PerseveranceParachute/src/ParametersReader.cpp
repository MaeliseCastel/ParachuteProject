#include <QFile>
#include <QDataStream>
#include "ParametersReader.h"

void ParametersReader::load(QString fileName) {
    QFile file(fileName);
    QChar qchar;
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> _sector;
    in >> _tracks;
    in >> _userText;
    in >> _blanks;
    in >> qchar;
    _defaultChar = qchar.toLatin1();
    in >> _color0BitBinary;
    in >> _color1BitBinary;
    in >> _color0BitParachute;
    in >> _color1BitParachute;
    file.close();
}

int ParametersReader::sector() const {
    return _sector;
}

int ParametersReader::tracks() const {
    return _tracks;
}

QString ParametersReader::userText() const {
    return _userText;
}

bool ParametersReader::blanks() const {
    return _blanks;
}

char ParametersReader::defaultChar() const {
    return _defaultChar;
}

QColor ParametersReader::bitBinary0Color() const {
    return _color0BitBinary;
}

QColor ParametersReader::bitBinary1Color() const {
    return _color1BitBinary;
}

QColor ParametersReader::bitParachute0Color() const {
    return _color0BitParachute;
}

QColor ParametersReader::bitParachute1Color() const {
    return _color1BitParachute;
}
