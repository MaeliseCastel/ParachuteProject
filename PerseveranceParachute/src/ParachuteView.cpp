#include "ParachuteView.h"
#include "Message.h"
#include <QPainterPath>
#include <QPen>
#include <QPainter>
#include <QPointF>
#include <QPolygon>
#include <QtMath>
#include <QMouseEvent>
#include <QWheelEvent>

ParachuteView::ParachuteView(QWidget *parent) :
    QWidget(parent) {
    _0BitColor = Qt::white;
    _1BitColor = Qt::red;
    _centralDiskColor = Qt::gray;
    _zoom = 1;
}

ParachuteView::~ParachuteView() {}

void ParachuteView::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton)
    {
        previousPointerPos = ev->pos();
    }
}

void ParachuteView::wheelEvent(QWheelEvent *ev) {
    QPoint angleScroll = ev->angleDelta();
    if(angleScroll.y() > 0) {
        _zoom += 0.1;
    } else if (angleScroll.y() < 0) {
        _zoom -= 0.1;
    }
    update();
}

void ParachuteView::mouseMoveEvent(QMouseEvent *ev) {
    if(ev->buttons() & Qt::LeftButton){
        currentPointerPos = currentPointerPos - previousPointerPos + ev->pos();
        previousPointerPos = ev->pos();
        ev->accept();
        update();
    }
}

void ParachuteView::computeDrawingParameters() {
    _translateX = width()/2;
    _translateY = height()/2;
    _nbSectors = _message->nbSectors();
    _nbTracks = _message->nbTracks();
    _r = _zoom * qMin(width()/2,height()/2);
    _a_e = 2*M_PI / _nbSectors;
    _w_e = _zoom * _r / (_nbTracks+1);
}

void ParachuteView::setModel(Message* message) {
    _message = message;
}

void ParachuteView::drawPolygon(QPainter& painter, int k) {
    if (!_message){
        return;
    }
    k += _nbSectors;

    double s = k % _nbSectors;
    double a_1 = s * _a_e;
    double a_2 = (s+1)*_a_e;
    int t = k/_nbSectors;
    double r_p = (t - 0.75)* _w_e;
    double r_q = (t+1 - 0.75) * _w_e;
    QPolygonF polygon;
    polygon << QPointF(r_p * cos(a_1) + _translateX,r_p * sin(a_1) + _translateY)
            << QPointF(r_q * cos(a_1) + _translateX,r_q * sin(a_1) + _translateY)
            << QPointF(r_q * cos(a_2) + _translateX,r_q * sin(a_2) + _translateY)
            << QPointF(r_p * cos(a_2) + _translateX, r_p * sin(a_2) + _translateY);

    QBrush brush;
    if (_message->getBitK(k-_nbSectors)){
        brush.setColor(_1BitColor);
        brush.setStyle(Qt::SolidPattern);
    } else {
        brush.setColor(_0BitColor);
        brush.setStyle(Qt::SolidPattern);
    }

    QPainterPath path;
    path.addPolygon(polygon);
    painter.drawPolygon(polygon);
    painter.fillPath(path, brush);
}

void ParachuteView::drawCentralDisk(QPainter& painter) {
    QPainterPath path;
    QBrush brush;
    path.addEllipse(QPointF(_translateX,_translateY),_w_e/4,_w_e/4);
    painter.drawEllipse(QPointF(_translateX,_translateY),_w_e/4,_w_e/4);
    brush.setColor(_centralDiskColor);
    brush.setStyle(Qt::SolidPattern);
    painter.fillPath(path, brush);
}

void ParachuteView::drawBackground(QPainter& painter){
    QRect rect(0,0,width(),height());
    painter.fillRect(rect,QColor(255,255,255));
    painter.drawRect(rect);
}

void ParachuteView::paintEvent(QPaintEvent*) {
    if (!_message){
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBackground(painter);
    painter.translate(currentPointerPos);
    computeDrawingParameters();
    for (int i = 0;i < _message->nbTracks()*_message->nbSectors();i++){
        drawPolygon(painter,i);
    }
    drawCentralDisk(painter);

    if ((_zoom == 1)&&(currentPointerPos.x() == 0)&&(currentPointerPos.y() == 0)){
        emit viewInDefaultState(false);
    } else {
        emit viewInDefaultState(true);
    }
}

QColor ParachuteView::bit0Color() {
    return _0BitColor;
}

QColor ParachuteView::bit1Color() {
    return _1BitColor;
}

void ParachuteView::changeColor0Bit(const QColor & newColor) {
    _0BitColor = newColor;
    update();
}

void ParachuteView::changeColor1Bit(const QColor & newColor) {
    _1BitColor = newColor;
    update();
}

void ParachuteView::resetView() {
    _zoom = 1;
    currentPointerPos = QPoint(0,0);
    previousPointerPos = QPoint(0,0);
    update();
}
