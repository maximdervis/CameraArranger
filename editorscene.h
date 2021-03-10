#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QEvent>
#include <QGraphicsView>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "room.h"

class room;

class editorscene : public QGraphicsView//, public MainWindow

{
public:
    explicit editorscene(QWidget *parent = 0, Ui::MainWindow* mainWindowUi = nullptr, room* given_room = nullptr);
    void addScene(QGraphicsScene* scene);
    void visualization(room* curRoom, QGraphicsScene *roomVisualization);
    //void mouseMoveEvent(QMouseEvent * ev);
    void mousePressEvent(QMouseEvent * ev);
    void mouseReleaseEvent(QMouseEvent * ev);
    void DrawWalls(QVector <tchk> line, int height);
    void EraseWalls(QVector <tchk> line);
    void drawCam(int i);
    void SetCam(int index, tchk coord);
    void removeCameraFromEditor(int index);
    void setPriority(QVector <tchk> line, int value);

    Ui::MainWindow* mainWindowUi;
    QGraphicsScene* scene;
    tchk start, end;
    room* given_room;
};

#endif // EDITORSCENE_H
