#ifndef CAMERAADDITION_H
#define CAMERAADDITION_H

#include <QDialog>
#include <QtXml>

namespace Ui {
class CameraAddition;
}

class CameraAddition : public QDialog
{
    Q_OBJECT

public:
    explicit CameraAddition(QWidget *parent = nullptr);
    ~CameraAddition();


    bool updateDBCamerasListFromXmlFile(QFile *file, QString tagName, QString attribute);

    QString getCameraName();
    qint32 getCamerasAmount();
    bool getAdditionStatus();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_addCameraToListButton_clicked();

    void on_CancelAdditionButton_clicked();

    void on_ConfirmAdditionButton_clicked();

//methods
private:
    void updateDBCamerasListFromRoot(QDomElement root, QString tagName, QString attribute);

private:
    Ui::CameraAddition *ui;
    QString addingCamerasName;
    qint32 addingCamerasAmount;
    bool additionConfirmed;

};

#endif // CAMERAADDITION_H
