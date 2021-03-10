#include "dynamicchangingcamera.h"
#include "ui_dynamicchangingcamera.h"

#include <QMessageBox>
#include <QKeyEvent>


DynamicChangingCamera::DynamicChangingCamera(qint32 roomWidth,
                                             qint32 roomLength,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DynamicChangingCamera)
{


    ui->setupUi(this);
    this->roomWidth = roomWidth;
    this->roomLength = roomLength;
}


DynamicChangingCamera::~DynamicChangingCamera()
{
    delete ui;
}

qint32 DynamicChangingCamera::getViewingAngle()
{
    return viewingAngleInput;
}
qint32 DynamicChangingCamera::getVisibilityRange()
{
    return visibilityRangeInput;
}
qint32 DynamicChangingCamera::getpositionX()
{
    return positionXInput;

}
qint32 DynamicChangingCamera::getPositionY()
{
    return positionYInput;

}
qint32 DynamicChangingCamera::getRotationAngle()
{
    return rotationAngleInput;

}

void DynamicChangingCamera::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
            return;
    DynamicChangingCamera::keyPressEvent(e);
}


void DynamicChangingCamera::on_saveButton_clicked()
{
    bool gotViewingAngle;
    bool gotPosX;
    bool gotPosY;
    bool gotRotationAngle;
    bool gotVisibilityRange;

    //trying to get values
    qint32 viewingAngle = ui->viewingAngleInputField->text().toInt(&gotViewingAngle);
    qint32 posX = ui->positionXInputField->text().toInt(&gotPosX);
    qint32 posY = ui->positionYInputField->text().toInt(&gotPosY);
    qint32 visibilityRange = ui->visibilityRangeInputField->text().toInt(&gotVisibilityRange);
    qint32 rotationAngle = ui->rotationAngleInputField->text().toInt(&gotRotationAngle);

    bool gotEverything = gotViewingAngle && gotPosX && gotPosY && gotRotationAngle && gotVisibilityRange;
    if(!gotEverything)
    {
        QMessageBox::critical(this, "Incorrect data", "Please, enter correct data");
        return;
    }else
    {
        if(viewingAngle < 1 || viewingAngle > 360)
        {
            QMessageBox::critical(this, "Incorrect data", "Viewing angle must be from 1 to 360\nplease, enter corrent data");
            return;
        }
        if(rotationAngle < 1 || rotationAngle > 360)
        {
            QMessageBox::critical(this, "Incorrect data", "Rotation angle must be from 1 to 360\nplease, enter corrent data");
            return;
        }
        if( posX < 0 || posX > roomWidth - 1 ||
            posY < 0 || posY > roomLength - 1)
        {
            QMessageBox::critical(this, "Incorrect data", "Position is outside the bounds of the room");
            return;
        }
        if(visibilityRange < 1)
        {
            QMessageBox::critical(this, "Incorrect data", "Visibility range must be greater than zero\nplease, enter corrent data");
            return;
        }

        viewingAngleInput = viewingAngle;
        positionXInput = posX;
        positionYInput = posY;
        rotationAngleInput = rotationAngle;
        visibilityRangeInput = visibilityRange;
        changingConfirmed = true;
    }
    this->close();
}

void DynamicChangingCamera::on_cancelButton_clicked()
{
    changingConfirmed = false;
    this->close();
}

bool DynamicChangingCamera::getChangingStatus()
{
    return changingConfirmed;
}




