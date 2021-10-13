#include "BitsView.h"
#include <QPainterPath>
#include <QPen>
#include <QPainter>
#include <QPointF>
#include <QPolygon>
#include <QtMath>
#include <QtGlobal>
#include "Message.h"

BitsView::BitsView(QWidget *parent) : QWidget(parent) {
    _0BitColor = Qt::white;
    _1BitColor = Qt::red;
}

BitsView::~BitsView() {}

void BitsView::setModel(Message* message) {
    _message=message;
}

void BitsView::drawBit(QPainter* painter, int k) {
    if (!_message){
        return;
    }

    int nbBits = _message->nbBits();
    int nbCarac = _message->messageLength();

    float pasW = width()/(2*(2*nbCarac+1));
    float pasH = height()/(2*(2*nbBits+1));
    float r = 0;
    if (pasH < pasW){
        r = pasH;
        pasW = (width() - 2*nbCarac*r)/(2*(nbCarac+1));
    } else {
        r = pasW;
        pasH = (height() - 2*nbBits*r)/(2*(nbBits+1));
    }
    float cx = (2*pasW + r) + (2*r + 2*pasW)*(k/nbBits);
    float cy = (2*pasH + r) + (2*r + 2*pasH)*(k%nbBits);
    QBrush brush;
    brush.setColor(_0BitColor);
    brush.setStyle(Qt::SolidPattern);
    if (_message) {
        if (_message->getBitK(k)){
            brush.setColor(_1BitColor);
            brush.setStyle(Qt::SolidPattern);
        }
    }
    QPainterPath path;
    path.addEllipse(QPointF(cx,cy),r,r);
    painter->drawPath(path);
    painter->fillPath(path, brush);
}

QColor BitsView::bit0Color() {
    return _0BitColor;
}

QColor BitsView::bit1Color() {
    return _1BitColor;
}

void BitsView::drawBackground(QPainter& painter){
    QRect rect(0,0,width(),height());
    painter.fillRect(rect,QColor(255,255,255));
    painter.drawRect(rect);
}

void BitsView::paintEvent(QPaintEvent*) {
    if (!_message){
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBackground(painter);
    for (int i = 0;i < _message->nbBits()*_message->messageLength();i++) {
        drawBit(&painter,i);
    }
}

void BitsView::changeColor0Bit(const QColor & newColor)
{
    _0BitColor = newColor;
    update();
}

void BitsView::changeColor1Bit(const QColor & newColor)
{
    _1BitColor = newColor;
    update();
}
