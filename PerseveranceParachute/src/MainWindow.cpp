#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ParametersWriter.h"
#include "ParametersReader.h"
#include <QSpinBox>
#include <QSlider>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(tr("Perseverance's Parachute"));
    setMinimumSize(500,500);

    char defaultChar = '@';
    int defaultTracks = 5;
    int defaultSector = 21;
    bool blanks = ui->addOffset->isChecked();

    initializeModel(defaultChar,defaultSector,defaultTracks,blanks);
    initializeErrorLabel();
    initializeSectorWidget(defaultSector);
    initializeTrackWidget(defaultTracks);
    initializeTextTransmission();
    linkModelAndView();
    connectActionMenu();
}

MainWindow::~MainWindow() {
    delete ui;
    delete _messageModel;
}

void MainWindow::initializeModel(char defaultChar, int defaultSector, int defaultTracks, bool blanks){
    _messageModel = new Message(defaultChar, defaultSector, defaultTracks, blanks, this);
    ui->parachuteView->setModel(_messageModel);
    ui->bitsView->setModel(_messageModel);
}

void MainWindow::initializeErrorLabel(){
    _errorLabelTimer = new QTimer(this);
    connect(ui->errorLabel, SIGNAL(linkActivated(const QString &)),this, SLOT(linkToChangeDefaultCharacterActivated(const QString)));
    connect(_errorLabelTimer,SIGNAL(timeout()),this,SLOT(hideErrorLabel()));
}

void MainWindow::initializeSectorWidget(int defaultNbSectors){
    ui->sectorsWidget->setTitle(tr("Sector number"));
    ui->sectorsWidget->setStepValue(_messageModel->nbBits());
    ui->sectorsWidget->setInitialMinMax(7, 35);
    ui->sectorsWidget->setCurrentValue(defaultNbSectors);
}

void MainWindow::initializeTrackWidget(int defaultNbTracks){
    ui->trackWidget->setTitle(tr("Track number"));
    ui->trackWidget->setStepValue(1);
    ui->trackWidget->setInitialMinMax(2, 9);
    ui->trackWidget->setCurrentValue(defaultNbTracks);
}

void MainWindow::initializeTextTransmission(){
    ui->textToEncode->setText(_messageModel->userText());
    resetValidateButtonStyle();

    connect(ui->validateButton, SIGNAL(clicked()), this, SLOT(processLineEditText()));
    connect(ui->textToEncode, SIGNAL(returnPressed()), this, SLOT(processLineEditText()));
    connect(ui->textToEncode,SIGNAL(textChanged(const QString)),this,SLOT(whenUserIsTyping()));
}

void MainWindow::linkModelAndView(){
    connect(ui->sectorsWidget, SIGNAL(valueUpdated(int)), _messageModel, SLOT(setSector(int)));
    connect(ui->trackWidget, SIGNAL(valueUpdated(int)), _messageModel, SLOT(setTrack(int)));

    connect(ui->parachuteView,SIGNAL(viewInDefaultState(bool)),ui->actionResetDefaultView,SLOT(setEnabled(bool)));

    connect(_messageModel, SIGNAL(notify()), ui->parachuteView, SLOT(update()));
    connect(_messageModel, SIGNAL(notify()), ui->bitsView, SLOT(update()));
    connect(_messageModel, SIGNAL(error(ErrorType, const QString &)),this, SLOT(processReceivedError(ErrorType, const QString &)));
}

void MainWindow::connectActionMenu(){
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(getDataFromFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveDataInFile()));
    connect(ui->about,SIGNAL(triggered()),this,SLOT(onAboutClicked()));
    connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionResetDefaultView,SIGNAL(triggered()),this,SLOT(onResetViewTriggered()));
    connect(ui->choose0BitColor, SIGNAL(triggered()), this, SLOT(chooseColor0Bit()));
    connect(ui->choose1BitColor, SIGNAL(triggered()), this, SLOT(chooseColor1Bit()));
    connect(ui->changeDefaultCharacter,SIGNAL(triggered()),this,SLOT(changeDefaultCharacter()));
    connect(ui->addOffset, SIGNAL(toggled(bool)), this, SLOT(offsetActionToogled(bool)));
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget* focusedWidget = QApplication::focusWidget();
    if (focusedWidget == ui->textToEncode){
        focusedWidget->clearFocus();
        resetValidateButtonStyle();
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::getDataFromFile() {
    resetTimer();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), ".", tr("Text Files (*.data)"),nullptr,QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty()) {
        _messageModel->disableNotifications();
        setWhenLoading(fileName);
        _messageModel->enableNotifications();
    }
}

void MainWindow::setWhenLoading(QString fileName) {
    ParametersReader reader;
    reader.load(fileName);
    ui->sectorsWidget->setCurrentValue(reader.sector());
    ui->trackWidget->setCurrentValue(reader.tracks());
    ui->textToEncode->setText(reader.userText());
    _messageModel->setBlanks(reader.blanks());
    ui->addOffset->setChecked(reader.blanks());
    _messageModel->setDefaultChar(reader.defaultChar());
    _messageModel->setUserText(reader.userText());
    resetValidateButtonStyle();
    ui->parachuteView->changeColor0Bit(reader.bitParachute0Color());
    ui->parachuteView->changeColor1Bit(reader.bitParachute1Color());
    ui->bitsView->changeColor0Bit(reader.bitBinary0Color());
    ui->bitsView->changeColor1Bit(reader.bitBinary1Color());
}

void MainWindow::saveDataInFile() {
    resetTimer();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Text Files (*.data)"),nullptr,QFileDialog::DontUseNativeDialog); 
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".data")){
            fileName+=".data";
        }
        ParametersWriter::save(fileName, _messageModel->nbSectors(),
                               _messageModel->nbTracks(),
                               _messageModel->userText(),
                               _messageModel->areBlanks(),
                               _messageModel->defaultChar(),
                               ui->bitsView->bit0Color(),
                               ui->bitsView->bit1Color(),
                               ui->parachuteView->bit0Color(),
                               ui->parachuteView->bit1Color());
    }
}

void MainWindow::processLineEditText() {
    resetTimer();
    resetValidateButtonStyle();
    ui->textToEncode->clearFocus();
    _messageModel->setUserText(ui->textToEncode->text());
}


void MainWindow::chooseColor0Bit() {
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid()){
        ui->parachuteView->changeColor0Bit(color);
        ui->bitsView->changeColor0Bit(color);
    }
}

void MainWindow::chooseColor1Bit() {
    QColor color = QColorDialog::getColor(Qt::red, this);
    if (color.isValid()){
        ui->parachuteView->changeColor1Bit(color);
        ui->bitsView->changeColor1Bit(color);
    }
}

void MainWindow::resetTimer() {
    _errorLabelTimer->setInterval(0);
}

void MainWindow::hideErrorLabel() {
    ui->errorLabel->setVisible(false);
}

void MainWindow::processReceivedError(ErrorType errorType, const QString &error) {
    if (errorType == ErrorType::TOO_LONG){
        ui->errorLabel->setText(error);
        ui->errorLabel->setVisible(true);
        _errorLabelTimer->setInterval(5000);
        _errorLabelTimer->start();
    } else if (errorType == ErrorType::WRONG_CHAR) {
        QString linkToAction = "<a href=\"#changeDefCarac\"><span style=\" text-decoration: underline; color:#0000ff;\">"
                               + tr("Default Character") + "</span></a>";
        QString tipForUser = tr(" (You may want to change the ");
        ui->errorLabel->setText(error + tipForUser + linkToAction + ")");
        ui->errorLabel->setVisible(true);
        _errorLabelTimer->setInterval(5000);
        _errorLabelTimer->start();
    }
}

void MainWindow::linkToChangeDefaultCharacterActivated(const QString &link) {
    if (link == "#changeDefCarac") {
        changeDefaultCharacter();
    }
}

void MainWindow::changeDefaultCharacter() {
    QInputDialog dialog(this);
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setWindowTitle(tr("Change default character"));
    dialog.setLabelText(tr("Type new default character : "));
    dialog.setTextValue(QString(_messageModel->defaultChar()));

    auto lineEdit = dialog.findChild<QLineEdit *>();
    lineEdit->setMaxLength(1);

    if (dialog.exec() == QDialog::Accepted) {
        QChar charVal(dialog.textValue()[0]);
        _messageModel->setDefaultChar(charVal.toLatin1());
    }
}

void MainWindow::offsetActionToogled(bool checked) {
    _messageModel->setBlanks(checked);
    ui->sectorsWidget->setStepValue(_messageModel->nbBits());
}

void MainWindow::whenUserIsTyping() {
    ui->validateButton->setStyleSheet("color: blue; font-weight: bold;");
}

void MainWindow::onAboutClicked() {
    QMessageBox about(QMessageBox::NoIcon, tr("About"),
                      tr("Perseverance Parachute v1.0 \n\n"
                          "March 2021 \n\n"
                          "Make a Binary codage to imit the one used for the Perseverance Parachute\n\n"
                          "ENSI2021\n"
                          "Maëlise Castel - Julien Zaïdi"),
                      QMessageBox::NoButton, this,Qt::Dialog);
    about.setIconPixmap(QPixmap(":/icons/logo.jpg"));
    about.exec();
}

void MainWindow::resetValidateButtonStyle() {
    ui->validateButton->setStyleSheet("color: default;font-weight: default;");
}

void MainWindow::onResetViewTriggered() {
    ui->parachuteView->resetView();
}

