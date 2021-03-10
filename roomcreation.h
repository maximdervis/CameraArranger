#ifndef ROOMCREATION_H
#define ROOMCREATION_H

#include <QDialog>

namespace Ui {
class RoomCreation;
}

class RoomCreation : public QDialog
{
    Q_OBJECT

public:
    explicit RoomCreation(QWidget *parent = nullptr);
    ~RoomCreation();

private slots:
    void on_confirmCreationButton_clicked();

    void on_cancelButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);

//getters and setters
public:
    bool getCreationStatus();
    qint32 getEnteredWidth();
    qint32 getEnteredLength();
    qint32 getEnteredHeight();

//attributes
private:
    Ui::RoomCreation *ui;
    bool creationConfirmed;
    qint32 enteredWidth;
    qint32 enteredLength;
    qint32 enteredHeight;




};

#endif // ROOMCREATION_H
