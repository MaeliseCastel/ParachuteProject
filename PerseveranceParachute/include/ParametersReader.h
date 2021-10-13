#ifndef PARAMETERSREADER_H
#define PARAMETERSREADER_H

#include <QColor>
#include <QObject>
#include <QString>

class ParametersReader {
public:
    void load(QString fileName);
    int sector() const;
    int tracks() const;
    QString userText() const;
    bool blanks() const;
    char defaultChar() const;
    QColor bitBinary0Color() const;
    QColor bitBinary1Color() const;
    QColor bitParachute0Color() const;
    QColor bitParachute1Color() const;

private:
    int _sector;
    int _tracks;
    QString _userText;
    bool _blanks;
    char _defaultChar;
    QColor _color0BitBinary;
    QColor _color1BitBinary;
    QColor _color0BitParachute;
    QColor _color1BitParachute;
};

#endif // PARAMETERSREADER_H
