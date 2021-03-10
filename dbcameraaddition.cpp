#include "dbcameraaddition.h"
#include "ui_dbcameraaddition.h"

#include <QMessageBox>
#include <QtXml>
#include <QKeyEvent>


DBCameraAddition::DBCameraAddition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBCameraAddition)
{
    ui->setupUi(this);
    this->additionConfirmed = false;
}

DBCameraAddition::~DBCameraAddition()
{
    delete ui;
}


bool DBCameraAddition::getAdditionStatus()
{
    return additionConfirmed;
}

void DBCameraAddition::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        event->accept();
    }else
       event->ignore();
}


void DBCameraAddition::on_additionToListCancelButton_clicked()
{
    //closing window on clicking close button
    this->close();
}

bool CameraExists(QString cameraName, QDomElement root)
{
    //checking if entered name already exists in xml file

    //getting list of elements by tag name
    QDomNodeList existingCameras = root.elementsByTagName("Camera");
    for(int i = 0; i < existingCameras.length(); i++)
    {
        //getting camera from list
        QDomNode currentCamera = existingCameras.at(i);
        //if current camera is element
        if(currentCamera.isElement())
            if(currentCamera.toElement().attribute("name") == cameraName)
            {
                return true;
                break;
            }
    }
    return false;
}

void DBCameraAddition::on_additionToListConfirmButton_clicked()
{
    //getting data from input fields:

    bool gotCameraName;
    QString cameraName = ui->cameraNameInputField->text();
    gotCameraName = !cameraName.isEmpty();

    //status variable that indicates if convertation to int was successful
    bool gotViewingAngle;
    qint32 viewingAngle = ui->viewingAngleInputField->text().toInt(&gotViewingAngle);

    //status variable that indicates if convertation to int was successful
    bool gotVisibilityRange;
    qint32 visibilityRange = ui->visibilityRangeInputField->text().toInt(&gotVisibilityRange);

    //checking if incorrect data was passed
    if(!gotViewingAngle || !gotVisibilityRange || !gotCameraName)
    {
        QMessageBox::critical(this, "Incorrect data", "Please, enter correct data");
        return;
    }
    else
    {
        if(viewingAngle > 360 || viewingAngle < 1)
        {
            QMessageBox::critical(this, "Incorrect data", "Viewing angle must be from 1 to 360\nplease, enter corrent data");
            return;
        }else
        if(visibilityRange < 1)
        {
            QMessageBox::critical(this, "Incorrect data", "Visibility range must be greater than zero\nplease, enter corrent data");
            return;
        }

        //adding camera to DB
        //creating xml document
        QDomDocument document("cameras");
        QFile file(QCoreApplication::applicationDirPath() + "/cameras.xml");

        //open or create new file
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QString message = "There was an error while opening/creating this file";
            QMessageBox::critical(this, "File opening error", message);
        }else
        {
            if(!document.setContent(&file))
            {
                //adding root element
                QDomElement cameras = document.createElement("Cameras");
                document.appendChild(cameras);

                file.close();
                QString message = "There was no file with cameras\nbut it was created";
                QMessageBox::information(this, "New file", message);
            }

            file.close();
        }

        //creating room elements
        QDomElement root = document.firstChildElement();

        //camera with entered name already exists in list
        if(CameraExists(cameraName, root))
        {
            QString message = "Camera with name " + cameraName + " already exists\nPlease, choose another name";
            QMessageBox::critical(this, "Camera already exists", message);
            return;
        }

        //creating new camera element
        QDomElement camera = document.createElement("Camera");
        camera.setAttribute("name", cameraName);
        camera.setAttribute("viewing_angle", viewingAngle);
        camera.setAttribute("visibility_range", visibilityRange);

        //adding camera to root
        root.appendChild(camera);

        //write to file
        //trying to open the file
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString message = "There was an error while opening/creating this file";
            QMessageBox::critical(this, "File opening error", message);
            return;
        }
        else
        {
            //writing everything
            QTextStream stream(&file);
            stream << document.toString();

            //and close the file
            file.close();

            additionConfirmed = true;
        }




        //closing window after camera added
        this->close();
    }
}


