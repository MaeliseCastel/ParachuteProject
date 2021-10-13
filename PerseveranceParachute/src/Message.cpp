#include <bitset>
#include <QtMath>
#include "Message.h"


const int Message::_nbBits = 7;

Message::Message(QObject * parent) : QObject(parent), _defaultChar('@'), _sector(21), _track(5), _blanks(false) {}

Message::Message(char defaultChar, int sector, int track, bool blanks, QObject * parent) :
    QObject(parent), _defaultChar(defaultChar), _sector(sector), _track(track), _blanks(blanks) {
    _userText = "Hello";
    if (_blanks) {
        _nbBlanks = 3;
    }
    encodeString();
}

bool Message::checkMessageLength(const QString & message) {
    if (message.length()*(_nbBits+_nbBlanks)<=(_sector*_track)){
        return true;
    }
    emit(error(ErrorType::TOO_LONG,tr("Your sentence is too long !")));
    return false;
}

QString Message::getBitOffsetPrefixIfRequired(){
    QString offset = "";
    if (_blanks) {
        for (int i=0; i<_nbBlanks; i++) {
            offset += "0";
        }
    }
    return offset;
}

QString Message::encodeChar(char charToEncode) {
    QString encodedCharBits = "";
    if ((_defaultChar <= charToEncode) && (charToEncode < _defaultChar+qPow(2, _nbBits))) {
        encodedCharBits = getBitOffsetPrefixIfRequired();
        int intToBinarize = (int)charToEncode - (int)_defaultChar;
        std::bitset<_nbBits> bits(intToBinarize);
        encodedCharBits += QString::fromStdString(bits.to_string());
    }
    return encodedCharBits;
}

void Message::processProhibitedCharInUserText(const QChar & prohibitedChar){
    _encodedString = "";
    QString prohibitedCharac = QString(prohibitedChar);
    if (prohibitedCharac == " "){
        prohibitedCharac = tr("[space]");
    }
    QString errorMessage = tr("Prohibited character : ")+ prohibitedCharac;
    emit(error(ErrorType::WRONG_CHAR,errorMessage));
}

void Message::encodeString() {
    QString currentCharBit;
    _encodedString = "";
    if (!checkMessageLength(_userText)){
        return;
    }
    for (int i=0; i<_userText.size(); i++) {
        currentCharBit = encodeChar(_userText.toStdString()[i]);
        if (currentCharBit=="") {
            processProhibitedCharInUserText(_userText[i]);
            break;
        }
        _encodedString+=currentCharBit;
    }
    sendNotificationIfEnabled();
}

bool Message::getBitK(int k) {
    if (k >= _encodedString.length()){
        return false;
    }
    return (_encodedString[k] == '1');
}

void Message::sendNotificationIfEnabled() {
  if (_notify) {
      emit(notify());
  }
}

int Message::maxChar() {
    return (int)_sector*_track/_nbBits;
}

int Message::messageLength() const {
    return _userText.length();
}

int Message::nbTracks() const {
    return _track;
}

int Message::nbSectors() const {
    return _sector;
}

int Message::nbBits() const {
    return _nbBits+_nbBlanks;
}

char Message::defaultChar() const {
    return _defaultChar;
}

QString Message::userText() const {
    return _userText;
}

bool Message::areBlanks() const {
    return _blanks;
}

void Message::setUserText(const QString & userText) {
    if (_userText != userText) {
        _userText = userText;
        encodeString();
    }
}

void Message::setSector(int sector) {
    _sector = sector;
    encodeString();
}

void Message::setTrack(int track) {
    _track = track;
    encodeString();
}

void Message::setDefaultChar(char defaultChar) {
    _defaultChar = defaultChar;
    encodeString();
}

void Message::setBlanks(bool blanks) {
    _blanks = blanks;
    if (_blanks) {
        _nbBlanks = 3;
    }
    else {
        _nbBlanks = 0;
    }
    encodeString();
}

void Message::enableNotifications() {
  _notify = true;
  encodeString();
}

void Message::disableNotifications() {
  _notify = false;
}
