#include "cameraaddition.h"
#include "ui_cameraaddition.h"
#include "mainwindow.h"

//connection DBCameraAddition window class
#include "dbcameraaddition.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QtXml>

CameraAddition::CameraAddition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraAddition)
{
    ui->setupUi(this);
}

CameraAddition::~CameraAddition()
{
    delete ui;
}

//processing clicking on create new camera in db button
void CameraAddition::on_addCameraToListButton_clicked()
{
    //creating a new variable that stores new DBCameraAddition window
    DBCameraAddition cameraDBAdditionWindow;
    //making new window modal (we can't use parent window while using this one)
    cameraDBAdditionWindow.setModal(true);
    //removing what's this and close button from QDialog window
    cameraDBAdditionWindow.setWindowFlags(cameraDBAdditionWindow.windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowCloseButtonHint);
    //opening just created window
    cameraDBAdditionWindow.exec();


    //if add button's been pressed
    if(cameraDBAdditionWindow.getAdditionStatus())
    {
        QFile camerasBdFile(QApplication::applicationDirPath() + "/cameras.xml");
        QString cameraTag = "Camera";
        QString nameAttribute = "name";
        if(!updateDBCamerasListFromXmlFile(&camerasBdFile, cameraTag, nameAttribute))
        {
            QMessageBox::critical(this, "Adding error", "Error while adding new camera to list\ntry to restart programme");
        }
    }
}

QString CameraAddition::getCameraName()
{
    return CameraAddition::addingCamerasName;
}

qint32 CameraAddition::getCamerasAmount()
{
    return CameraAddition::addingCamerasAmount;
}

bool CameraAddition::getAdditionStatus()
{
    return CameraAddition::additionConfirmed;
}


void CameraAddition::on_CancelAdditionButton_clicked()
{
    additionConfirmed = false;
    //closing window on clicking cancel button
    this->close();
}

void CameraAddition::on_ConfirmAdditionButton_clicked()
{
    if(ui->camerasList->count() == 0)
    {
        QMessageBox::critical(this, "Addition \"empty\" camera",
                              "The list with cameras is empty so you can't add camera from it\n"
                                                                 "Press \"Cancel\" or add at least one camera");
        return;
    }

    //getting name from comboBox widget (camerasList)
    addingCamerasName = ui->camerasList->currentText();


    //status variable
    bool gotCamerasAmount;
    //getting cameras amount
    addingCamerasAmount = ui->addingCamerasAmountInputField->text().toInt(&gotCamerasAmount);
    if(!gotCamerasAmount)
    {
        QMessageBox::critical(this, "Incorrect data", "Please, enter correct data");
        return;
    }else
    {
        if(addingCamerasAmount < 1)
        {
            QMessageBox::critical(this, "Incorrect data", "Adding cameras amount must be greater than zero");
            return;
        }

        additionConfirmed = true;
        //closing window after passing data
        this->close();
    }
}

//updating list from cirtain file
bool CameraAddition::updateDBCamerasListFromXmlFile(QFile *file, QString tagName, QString attribute)
{
    //create a new document
    QDomDocument document("cameras");

    //try to open a new file
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }else
    {
        //fill document with data from file
        if(!document.setContent(file))
        {
            //close file and return false because nothing's done
            file->close();
            return false;
        }
        //close file
        file->close();
    }

    //update cameras in list
    QDomElement camerasRoot = document.firstChildElement();
    updateDBCamerasListFromRoot(camerasRoot, tagName, attribute);
    return true;
}

void CameraAddition::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        event->accept();
    }else
       event->ignore();
}



//get info about cameras from db and pass their names to combobox
void CameraAddition::updateDBCamerasListFromRoot(QDomElement root, QString tagName, QString attribute)
{
    //removing all items from list to fill it with new
    ui->camerasList->clear();
    //getting list of elements by tag name
    QDomNodeList newItems = root.elementsByTagName(tagName);

    //iterating over all found elements with such a tag
    for(int i = 0; i < newItems.length(); i++)
    {
        QDomNode currentItemNode = newItems.at(i);

        if(currentItemNode.isElement())
        {
            //converting item node to element
            QDomElement currentItemElemenet = currentItemNode.toElement();
            //getting name of each item
            QString currentItemName = currentItemElemenet.attribute(attribute);
            //adding each item's name to list of cameras
            ui->camerasList->addItem(currentItemName);
        }

    }
}






