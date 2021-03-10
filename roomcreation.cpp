#include "roomcreation.h"
#include "ui_roomcreation.h"

#include <QMessageBox>
#include <QKeyEvent>


RoomCreation::RoomCreation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomCreation)
{
    ui->setupUi(this);
}

RoomCreation::~RoomCreation()
{
    delete ui;
}

void RoomCreation::on_confirmCreationButton_clicked()
{
    QString enteredWidthData = ui->widthInputField->text();
    QString enteredLengthData = ui->lengthInputField->text();
    QString enteredHeightData = ui->heightInputField->text();

    bool gotWidth;
    qint32 enteredWidthDataInt = enteredWidthData.toInt(&gotWidth);

    bool gotLength;
    qint32 enteredLengthDataInt = enteredLengthData.toInt(&gotLength);

    bool gotHeight;
    qint32 enteredHeightDataInt = enteredHeightData.toInt(&gotHeight);

    if(!gotLength || !gotWidth || !gotHeight)
    {
        QMessageBox::critical(this, "Incorrect data", "Width, length and height must be integers greater than zero. Please, enter corrent data");
        return;
    }else
    {
        if(enteredLengthDataInt <= 0 || enteredWidthDataInt <= 0 || enteredHeightDataInt <= 0)
        {
            QMessageBox::critical(this, "Incorrect data", "Width, length and height must be greater than zero. Please, enter corrent data");
            return;
        }
        enteredLength = enteredLengthDataInt;
        enteredWidth = enteredWidthDataInt;
        enteredHeight = enteredHeightDataInt;
        creationConfirmed = true;
    }
    this->close();
}

void RoomCreation::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        event->accept();
    }else
       event->ignore();
}

void RoomCreation::on_cancelButton_clicked()
{
    creationConfirmed = false;
    this->close();
}

bool RoomCreation::getCreationStatus()
{
    return creationConfirmed;
}

qint32 RoomCreation::getEnteredLength()
{
    return enteredLength;
}

qint32 RoomCreation::getEnteredHeight()
{
    return enteredHeight;
}

qint32 RoomCreation::getEnteredWidth()
{
    return enteredWidth;
}

