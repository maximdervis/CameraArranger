#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    bool getSettingsChangingStatus();
    qint32 getPopulationsAmnt();

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Settings *ui;

    bool settingsChangingConfirmed;
    qint32 populationsAmnt;

};

#endif // SETTINGS_H
