#ifndef DBCAMERAADDITION_H
#define DBCAMERAADDITION_H

#include <QDialog>
#include <QtXml>

namespace Ui {
class DBCameraAddition;
}

class DBCameraAddition : public QDialog
{
    Q_OBJECT

public:
    explicit DBCameraAddition(QWidget *parent = nullptr);
    ~DBCameraAddition();
    bool getAdditionStatus();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_additionToListCancelButton_clicked();

    void on_additionToListConfirmButton_clicked();

private:
    Ui::DBCameraAddition *ui;
    bool additionConfirmed;
};

#endif // DBCAMERAADDITION_H
