#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QCheckBox>
#include <QGraphicsScene>

#include <QGraphicsItem>
#include <QPainter>
#include <QListWidgetItem>
#include <QInputDialog>
#include "room.h"
#include "editorscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class editorscene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     Ui::MainWindow *ui;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupRoomGrid();
    void clearRoomEditor();
    bool cellIsWall(int x, int y);
    qint32 getEditorWidth();
    qint32 getEditorLength();
    qint32 getEditorHeight();
    void FillNameToSpecsMap();

private slots:
    void on_addCameraButton_clicked();

    void on_removeCameraButton_clicked();

    void on_actionNew_room_triggered();

    void on_actionRun_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_room_triggered();

    void on_camerasListWidget_itemDoubleClicked(QListWidgetItem *item);

//attrubutes

    void on_actionSettings_triggered();

    void on_WallButton_clicked();

    void on_EraseButton_clicked();

    void on_CameraButton_clicked();

    void on_PriorityButton_clicked();

    void on_camerasListWidget_itemClicked(QListWidgetItem *item);

    void on_rotationSpinBox_valueChanged(int arg1);

private:
    qint32 editorWidth;
    qint32 editorLength;
    qint32 editorHeight;
    qint32 editorCellSize;

    qint32 curBenefit;

    QMap<QString, cam> nameToSpecs;
    //QVector<QVector<QCheckBox*>> editor;
    QVector<QVector<int> > map_of_room;
    QVector <cam> v_cams;

    bool roomIsCreated;
    bool arrangementFound;
    bool sceneIsCreated;

    qint32 populationsAmnt;
    int activeCameraIndex = -1;

private:
    editorscene *graphicsView;
    QGraphicsScene *scene;
    QGraphicsRectItem *rect;
    room *given_room;



};
#endif // MAINWINDOW_H
