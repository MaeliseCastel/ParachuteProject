#ifndef PARACHUTEVIEW_H
#define PARACHUTEVIEW_H

#include <QWidget>

class Message;

class ParachuteView : public QWidget {
    Q_OBJECT

public:
    explicit ParachuteView(QWidget *parent = nullptr);
    void setModel(Message* message);
    void changeColor0Bit(const QColor & newColor);
    void changeColor1Bit(const QColor & newColor);
    QColor bit0Color();
    QColor bit1Color();
    ~ParachuteView();    

protected:
    void paintEvent(QPaintEvent*);
    void wheelEvent(QWheelEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

public slots :
    void resetView();

signals:
    void viewInDefaultState(bool isInDefaultState);

private :
    void drawPolygon(QPainter& painter, int k);
    void computeDrawingParameters();
    void drawCentralDisk(QPainter& painter);
    void drawBackground(QPainter& painter);

    Message* _message = nullptr;

    int _nbTracks;
    int _nbSectors;
    int _translateX;
    int _translateY;
    double _r;
    double _a_e;
    double _w_e;

    QColor _0BitColor;
    QColor _1BitColor;
    QColor _centralDiskColor;

    double _zoom;
    QPoint currentPointerPos, previousPointerPos;
};

#endif // PARACHUTEVIEW_H
