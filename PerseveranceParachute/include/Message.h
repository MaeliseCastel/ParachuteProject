#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <string>
#include <QObject>
#include <QString>

enum ErrorType {
    TOO_LONG,
    WRONG_CHAR
};

class Message : public QObject {

    Q_OBJECT

public:
        explicit Message(QObject * parent = nullptr);
        Message(char defaultChar, int sector, int track, bool blanks,
                QObject * parent = nullptr);
        bool getBitK(int k);
        int messageLength() const;
        int nbTracks() const;
        int nbSectors() const;
        int nbBits() const;
        char defaultChar() const;
        QString userText() const;
        bool areBlanks() const;

public slots:
        void setUserText(const QString & userText);
        void setSector(int sector);
        void setTrack(int track);
        void setDefaultChar(char defaultChar);
        void setBlanks(bool blanks);
        void encodeString();
        void enableNotifications();
        void disableNotifications();

signals:
        void notify();
        void error(ErrorType errorType, const QString & message);

private:
        char _defaultChar;
        static const int _nbBits;
        int _nbBlanks = 0;
        int _sector;
        int _track;
        bool _notify = true;
        bool _blanks;
        QString _userText;
        QString _encodedString;

        QString encodeChar(char charToEncode);
        void sendNotificationIfEnabled();
        int maxChar();
        bool checkMessageLength(const QString & message);
        void processProhibitedCharInUserText(const QChar & prohibitedChar);
        QString getBitOffsetPrefixIfRequired();
};

#endif // MESSAGEMODEL_H
