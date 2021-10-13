#ifndef BITSVIEW_H
#define BITSVIEW_H

#include <QWidget>
class Message;

class BitsView : public QWidget {
    Q_OBJECT

public:
    explicit BitsView(QWidget *parent = nullptr);
    void setModel(Message* message);
    void changeColor0Bit(const QColor & newColor);
    void changeColor1Bit(const QColor & newColor);
    ~BitsView();
    QColor bit0Color();
    QColor bit1Color();

protected:
    void paintEvent(QPaintEvent*);

private :
    void drawBit(QPainter* painter, int k);
    void drawBackground(QPainter& painter);

    Message* _message;
    QColor _0BitColor;
    QColor _1BitColor;
};

#endif // BITSVIEW_H
