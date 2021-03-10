#ifndef CAMERAINFORMATION_H
#define CAMERAINFORMATION_H

#include <QDialog>

namespace Ui {
class CameraInformation;
}

class CameraInformation : public QDialog
{
    Q_OBJECT
public:
    explicit CameraInformation(qint32 roomWidth,
                               qint32 roomLength,
                               QWidget *parent = nullptr);
    ~CameraInformation();
    void setName(QString name);
    void setViewingAngle(qint32 viewingAngle);
    void setVisibilityRange(qint32 visibilityRange);
    void setPositionX(qint32 positionX);
    void setPositionY(qint32 positionY);
    void setRotationAngle(qint32 rotationAngle);

    QString getName();
    qint32 getViewingAngle();
    qint32 getVisibilityRange();
    qint32 getPositionX();
    qint32 getPositionY();
    qint32 getRotationAngle();

    void setupInfo();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_confirmButton_clicked();

    void on_changeSettingsButton_clicked();

private:
    Ui::CameraInformation *ui;
    QString name;
    qint32 viewingAngle;
    qint32 visibilityRange;
    qint32 positionX, positionY;
    qint32 rotationAngle;

    qint32 roomWidth;
    qint32 roomLength;
};

#endif // CAMERAINFORMATION_H
