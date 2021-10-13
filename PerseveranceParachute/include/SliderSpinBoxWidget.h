#ifndef PERSEVERANCEPARACHUTE_SLIDERSPINBOXWIDGET_H
#define PERSEVERANCEPARACHUTE_SLIDERSPINBOXWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SliderSpinBoxWidget; }
QT_END_NAMESPACE

class SliderSpinBoxWidget : public QWidget {
    Q_OBJECT

public:
    explicit SliderSpinBoxWidget(QWidget *parent = nullptr);

    void setTitle(const QString& title);
    void setInitialMinMax(int min, int max);
    void setCurrentValue(int curr);
    void setStepValue(int step);

    ~SliderSpinBoxWidget() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

public slots :
    void updatedSliderValue();
    void updatedSpinBoxValue();
    void updatedSpinBoxValue(const QString &text);

signals :
    void valueUpdated(int value);

private:
    void resetMinMax();

    Ui::SliderSpinBoxWidget *ui;
    int _min0;
    int _max0;
    int _step = 1;
};


#endif //PERSEVERANCEPARACHUTE_SLIDERSPINBOXWIDGET_H
