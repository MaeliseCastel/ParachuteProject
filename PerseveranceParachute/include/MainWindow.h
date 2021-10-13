#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Message.h"

class ParachuteView;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);

public slots :
    void getDataFromFile();
    void saveDataInFile();
    void processLineEditText();
    void chooseColor0Bit();
    void chooseColor1Bit();
    void processReceivedError(ErrorType errorType, const QString & message);
    void linkToChangeDefaultCharacterActivated(const QString &link);
    void changeDefaultCharacter();
    void hideErrorLabel();
    void offsetActionToogled(bool checked);
    void whenUserIsTyping();
    void onAboutClicked();
    void onResetViewTriggered();


private:
    Ui::MainWindow *ui;
    Message* _messageModel;
    QTimer* _errorLabelTimer;


    void connectActionMenu();

    void resetTimer();
    void resetValidateButtonStyle();
    void setWhenLoading(QString fileName);
    void initializeModel(char defaultChar, int defaultSector, int defaultTracks, bool blanks);
    void initializeTrackWidget(int defaultNbTracks);
    void initializeSectorWidget(int defaultNbSectors);
    void initializeErrorLabel();
    void initializeTextTransmission();
    void linkModelAndView();
};
#endif // MAINWINDOW_H
