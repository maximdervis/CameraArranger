#include "camerainformation.h"
#include "ui_camerainformation.h"

#include "dynamicchangingcamera.h"

#include <QKeyEvent>

#include <QString>

CameraInformation::CameraInformation(qint32 roomWidth,
                                     qint32 roomLength,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraInformation)
{
    ui->setupUi(this);
    this->roomWidth = roomWidth;
    this->roomLength= roomLength;

}

CameraInformation::~CameraInformation()
{
    delete ui;
}

void CameraInformation::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        event->accept();
    }else
       event->ignore();
}

//setters (getting information from another place)
void CameraInformation::setName(QString name)
{
    this->name = name;
}
void CameraInformation::setViewingAngle(qint32 viewingAngle)
{
    this->viewingAngle = viewingAngle;
}
void CameraInformation::setVisibilityRange(qint32 visibilityRange)
{
    this->visibilityRange = visibilityRange;
}
void CameraInformation::setPositionX(qint32 positionX)
{
    this->positionX = positionX;
}
void CameraInformation::setPositionY(qint32 positionY)
{
    this->positionY = positionY;
}
void CameraInformation::setRotationAngle(qint32 rotationAngle)
{
    this->rotationAngle = rotationAngle;
}
void CameraInformation::setupInfo()
{
    //setting information
    ui->camearaNameInfo->setText(name);
    ui->viewingAngleInfo->setText(QVariant(viewingAngle).toString());
    ui->visibilityRangeInfo->setText(QVariant(visibilityRange).toString());

    QString positionText = "{ " + QVariant(positionX).toString() + " ; " \
                                + QVariant(positionY).toString() + " }";
    ui->positionInfo->setText(positionText);

    ui->rotationAngleInfo->setText(QVariant(rotationAngle).toString());


}

void CameraInformation::on_confirmButton_clicked()
{
    this->close();
}

void CameraInformation::on_changeSettingsButton_clicked()
{
    //getting information about cameras

    //creating a new variable that stores new RoomCreation window
    DynamicChangingCamera dynamicChangingCameraWindow(roomWidth, roomLength);
    //making new window modal (we can't use parent window while using this one)
    dynamicChangingCameraWindow.setModal(true);
    //removing what's this and close button from QDialog window
    dynamicChangingCameraWindow.setWindowFlags(dynamicChangingCameraWindow.windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowCloseButtonHint);
    //opening just created window
    dynamicChangingCameraWindow.exec();

    if(dynamicChangingCameraWindow.getChangingStatus())
    {
        //getting info
        qint32 viewingAngle = dynamicChangingCameraWindow.getViewingAngle();
        qint32 visibilityRange = dynamicChangingCameraWindow.getVisibilityRange();
        qint32 positionX = dynamicChangingCameraWindow.getpositionX();
        qint32 positionY = dynamicChangingCameraWindow.getPositionY();
        qint32 rotationAngle = dynamicChangingCameraWindow.getRotationAngle();

        //changing
        this->viewingAngle = viewingAngle;
        this->visibilityRange = visibilityRange;
        this->positionX = positionX;
        this->positionY = positionY;
        this->rotationAngle = rotationAngle;
        setupInfo();
    }

}

QString CameraInformation::getName()
{
    return ui->camearaNameInfo->text();
}
qint32 CameraInformation::getViewingAngle()
{
    return ui->viewingAngleInfo->text().toInt();
}
qint32 CameraInformation::getVisibilityRange()
{
    return ui->visibilityRangeInfo->text().toInt();
}
qint32 CameraInformation::getPositionX()
{
    QString positionStr = ui->positionInfo->text();
    positionStr.remove(0, 1);
    positionStr.remove(positionStr.length() - 1, 1);

    QStringList coordinates = positionStr.split(';');
    return coordinates.front().toInt();
}
qint32 CameraInformation::getPositionY()
{
    QString positionStr = ui->positionInfo->text();
    positionStr.remove(0, 1);
    positionStr.remove(positionStr.length() - 1, 1);

    QStringList coordinates = positionStr.split(';');
    return coordinates.back().toInt();

}
qint32 CameraInformation::getRotationAngle()
{
    return ui->rotationAngleInfo->text().toInt();
}
