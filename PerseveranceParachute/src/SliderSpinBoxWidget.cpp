#include "SliderSpinBoxWidget.h"
#include "ui_SliderSpinBoxWidget.h"


SliderSpinBoxWidget::SliderSpinBoxWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::SliderSpinBoxWidget) {
    ui->setupUi(this);
    connect(ui->hSlider,SIGNAL(valueChanged(int)),this,SLOT(updatedSliderValue()));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    connect(ui->spinBox,SIGNAL(textChanged(const QString)),this,SLOT(updatedSpinBoxValue(const QString)));
#else
    connect(ui->spinBox,SIGNAL(valueChanged(const QString)),this,SLOT(updatedSpinBoxValue(const QString)));
#endif
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(updatedSpinBoxValue()));
}

void SliderSpinBoxWidget::setTitle(const QString& title) {
    ui->title->setText(title);
}

void SliderSpinBoxWidget::setInitialMinMax(int min, int max) {
    _min0 = min;
    _max0 = max;
    resetMinMax();
}

void SliderSpinBoxWidget::setStepValue(int step) {
    _step = step;
    ui->spinBox->setSingleStep(step);
    resetMinMax();
    updatedSpinBoxValue();
    updatedSliderValue();
}

void SliderSpinBoxWidget::resetMinMax() {
    int realMin = qMax(_step,(_min0/_step)*_step);
    int realMax = qMax((_max0/_step)*_step,_step);

    ui->hSlider->setMinimum(realMin/_step);
    ui->hSlider->setMaximum(realMax/_step);
    ui->spinBox->setMinimum(realMin);
    ui->spinBox->setMaximum(realMax);
}

void SliderSpinBoxWidget::setCurrentValue(int curr) {
    ui->hSlider->setValue(curr);
    ui->spinBox->setValue(curr);
}

void SliderSpinBoxWidget::updatedSpinBoxValue() {
    ui->hSlider->setValue(ui->spinBox->value()/_step);
    emit valueUpdated(ui->spinBox->value());
}

void SliderSpinBoxWidget::updatedSpinBoxValue(const QString& text) {
    int textInt = text.toInt();
    ui->spinBox->setValue((textInt/_step)*_step);
    updatedSpinBoxValue();
}

void SliderSpinBoxWidget::updatedSliderValue() {
    ui->spinBox->setValue(ui->hSlider->value()*_step);
    emit valueUpdated(ui->hSlider->value()*_step);
}

void SliderSpinBoxWidget::mousePressEvent(QMouseEvent *event) {
    QWidget* focusedWidget = QApplication::focusWidget();
    if (focusedWidget == ui->spinBox){
        focusedWidget->clearFocus();
    }
    QWidget::mousePressEvent(event);
}

SliderSpinBoxWidget::~SliderSpinBoxWidget() {
    delete ui;
}

