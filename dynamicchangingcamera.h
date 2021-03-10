#ifndef DYNAMICCHANGINGCAMERA_H
#define DYNAMICCHANGINGCAMERA_H

#include <QDialog>

namespace Ui {
class DynamicChangingCamera;
}

class DynamicChangingCamera : public QDialog
{
    Q_OBJECT

public:
    explicit DynamicChangingCamera(qint32 roomWidth,
                                   qint32 roomLength,
                                   QWidget *parent = nullptr);
    ~DynamicChangingCamera();

    qint32 getViewingAngle();
    qint32 getVisibilityRange();
    qint32 getpositionX();
    qint32 getPositionY();
    qint32 getRotationAngle();

    bool getChangingStatus();

protected:
    void keyPressEvent(QKeyEvent *event);


private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::DynamicChangingCamera *ui;




//attributes
private:
    qint32 viewingAngleInput;
    qint32 visibilityRangeInput;
    qint32 positionXInput;
    qint32 positionYInput;
    qint32 rotationAngleInput;

    qint32 roomWidth;
    qint32 roomLength;

    bool changingConfirmed;
};

#endif // DYNAMICCHANGINGCAMERA_H
