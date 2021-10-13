#ifndef PARAMETERSWRITER_H
#define PARAMETERSWRITER_H

#include <QColor>
#include <QString>

class ParametersWriter {
public:
    static void save(QString fileName, int sector, int tracks,
                     QString userText, bool blanks, char defaultChar,
                     QColor color0BitBinary, QColor color1BitBinary,
                     QColor color0BitParachute, QColor color1BitParachute);
};

#endif // PARAMETERSWRITER_H
