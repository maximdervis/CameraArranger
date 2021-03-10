#include "settings.h"
#include "ui_settings.h"

#include <QMessageBox>
#include <QKeyEvent>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

bool Settings::getSettingsChangingStatus()
{
    return settingsChangingConfirmed;
}

qint32 Settings::getPopulationsAmnt()
{
    return populationsAmnt;
}

void Settings::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        event->accept();
    }else
       event->ignore();
}


void Settings::on_confirmButton_clicked()
{
    bool gotPopulationsAmnt;
    populationsAmnt = ui->precisionInputField->text().toInt(&gotPopulationsAmnt);

    if(!gotPopulationsAmnt)
    {
        QMessageBox::critical(this, "Incorrect data", "Please, enter correct data");
        return;
    }else
    {
        if(populationsAmnt <= 0)
        {
            QMessageBox::critical(this, "Incorrect data", "Presicion should be not less than one");
            return;
        }else
        {
            settingsChangingConfirmed = true;
            this->close();
        }
    }
}

void Settings::on_cancelButton_clicked()
{
    settingsChangingConfirmed = false;
    this->close();
}
