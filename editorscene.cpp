#include "editorscene.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include "room.h"
#include <QMessageBox>

struct tchk;

editorscene::editorscene(QWidget *parent, Ui::MainWindow* mainWindowUi, room* given_room) :
    QGraphicsView(parent)
{
    this->mainWindowUi = mainWindowUi;
    this->given_room = given_room;
}

void editorscene::addScene(QGraphicsScene *scene)
{
    this->scene = scene;
}

void editorscene::visualization(room* curRoom, QGraphicsScene *roomVisualization)//QVector <QVector<bool>> wall_map, QVector <cam> coord,
                    //QVector < QVector<double> > benefit_map,
                   //qint32 max_saturation, QGraphicsScene *roomVisualization)
{

    QBrush blackBrush (Qt::black);
    QBrush redBrush (Qt::red);
    QBrush whiteBrush (Qt::white);

    QPen blackPen (Qt::black);
    blackPen.setWidth(2);
    QPen grayPen(QColor(196, 196, 196));
    QPen transparentPen(QColor(0,0,0,0));

    QVector <QVector<int>> wall_map = curRoom->cam_map;
    QVector <cam> coord = curRoom->cams;
    QVector < QVector<double> > benefit_map = curRoom->getBenefitMap();

    int max_saturation = curRoom->getMaxSizeOfView();

    for (int i = 0; i < wall_map.size(); i++)
    {
        for (int j = 0; j < wall_map[0].size(); j++)
        {
            cout << wall_map[i][j] << " ";
        }
        cout << endl;
    }

    //white background
    roomVisualization->addRect(0,0,20*wall_map[0].size(),20*wall_map.size(),blackPen,whiteBrush);
    QPen curPen;
    for (qint32 i = 0; i < wall_map.size(); i++)
    {
        for (qint32 j = 0; j < wall_map[i].size(); j++)
        {
            //cout << i << " " << j << endl;
            if (wall_map[i][j] > 0)
            {
                QBrush curBrush ((QColor(0, 0, 0, round((double)wall_map[i][j]/given_room->height*255))));
                roomVisualization->addRect(20*j,20*i,20,20,blackPen,curBrush);
            }
            else
            if (wall_map[i][j] == -1)
            {
                roomVisualization->addRect(20*j,20*i,20,20,blackPen,blackBrush);//walls
            }
            else
            {

                if (wall_map[i][j] < -1)
                {
                    //cout << mainWindowUi->prioritySpinBox->value() << endl;
                    //cout << 1 << endl;
                    curPen.setColor(QColor(255, 0, 0, -1*(wall_map[i][j])+155));
                    //cout << 2 << endl;
                    curPen.setWidth(2);
                }
                else
                {
                    curPen.setColor(Qt::black);
                    curPen.setWidth(2);
                }

                if (benefit_map[i][j] > 0)
                {
                    //benefit
                    //cout << 3 << endl;
                    //cout << benefit_map[i][j] << endl;
                    QBrush curBrush ((QColor(245, 234, 137, round((double)benefit_map[i][j]/max_saturation*255))));
                    //cout << 4 << endl;
                    roomVisualization->addRect(20*j,20*i,20,20,curPen,curBrush);
                    //cout << 5 << endl;
                }
                else
                {
                    //blind zones
                    roomVisualization->addRect(20*j,20*i,20,20,curPen,whiteBrush);
                    roomVisualization->addLine(int (20*j+5), int (20*i+5), int (20*j+15), int (20*i+15), grayPen);
                    roomVisualization->addLine(int (20*j+15), int (20*i+5), int (20*j+5), int (20*i+15), grayPen);
                }
            }
        }
    }
    //cout << 8 << endl;

    //cameras
    for (qint32 z = 0; z < coord.size(); z++)
    {
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(20*coord[z].poz.x+2, 20*coord[z].poz.y+2, 16, 16);

        qint32 right_side = coord[z].povorot+coord[z].angle;
        if (right_side <= 90)
        {
            item->setStartAngle((90-right_side)*16);
        }
        else
        {
            item->setStartAngle((450-right_side)*16);
        }
        item->setSpanAngle(coord[z].angle*16);
        if (coord[z].active)
            item->setBrush(QBrush(Qt::green));
        else
        item->setBrush(QBrush(Qt::red));
        roomVisualization->addItem(item);
    }
    //cout << 9 << endl;
}

void GetLine(tchk startPoint, tchk endPoint, QVector<tchk>& result)
{
    int x0 = startPoint.x;
    int x1 = endPoint.x;

    int y0 = startPoint.y;
    int y1 = endPoint.y;

    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    result.clear();

    //result.push_back(startPoint);

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = 0;
    int ystep;
    int y = y0;
    if (y0 < y1) ystep = 1; else ystep = -1;
    for (int x = x0; x <= x1; x++) {
        if (steep) result.push_back({y, x});
        else result.push_back({x, y});
        error += deltay;
        if (2 * error >= deltax) {
            y += ystep;
            error -= deltax;
        }
    }
    //result.push_back(endPoint);
}

void editorscene::DrawWalls(QVector <tchk> line, int height)//, room *given_room, QGraphicsScene* scene)
{
    QBrush whiteBrush (Qt::white);
    QBrush blackBrush(Qt::black);
    QPen blackPen (Qt::black);
    blackPen.setWidth(2);
    cout << "drawing" << endl;
    for (int i = 0; i < given_room->cam_map.size(); i++)
    {
        for (int j = 0; j < given_room->cam_map[0].size(); j++)
        {
            cout << given_room->cam_map[i][j] << "\t";
        }
        cout << endl;
    }

    for (int i = 0; i < line.size(); i++)
    {
        cout << "Line " << i << " " << line[i].x << " " << line[i].y << endl;
        if (line[i].x > 0 && line[i].y > 0 && line[i].x < given_room->cam_map[0].size()-1 && line[i].y < given_room->cam_map.size()-1)
        {
            cout << 1 << endl;
                if (height == given_room->height)
                    given_room->cam_map[line[i].y][line[i].x] = -1;
                else
                {
                    given_room->cam_map[line[i].y][line[i].x] = height;
                }
                cout << 2 << endl;
                for (int j = 0; j < given_room->cams.size(); j++)
                {
                    if (given_room->cams[j].poz.x == line[i].x && given_room->cams[j].poz.y == line[i].y)
                    {
                        //cout << "Pos: " << i << " " << given_room->cams[i].poz.x << " " << given_room->cams[i].poz.y << endl;
                        while (given_room->cam_map[given_room->cams[j].poz.y][given_room->cams[j].poz.x] == -1 || given_room->cam_map[given_room->cams[j].poz.y][given_room->cams[j].poz.x] > 0)//-1)
                        {
                            //cout << given_room->cams[i].poz.y << " " << given_room->cams[i].poz.x << endl;
                            if(given_room->cam_map[0].size() > 3)
                                given_room->cams[j].poz.x = rand()%(given_room->cam_map[0].size() - 2) + 1;
                            else
                                given_room->cams[j].poz.x = 1;

                            if(given_room->cam_map.size() > 3)
                                given_room->cams[j].poz.y = rand()%(given_room->cam_map.size() - 2) + 1;
                            else
                                given_room->cams[j].poz.y = 1;
                        }
                    }
                }
                /*rect = scene->addRect(20*line[i].x,20*line[i].y,20,20,blackPen,blackBrush);
                cout << "!!" << endl;
                for (int j = 0; j < given_room->cams.size(); j++)
                {
                    cout << j << " " << given_room->cams[j].poz.x << " " << given_room->cams[j].poz.y << endl;
                    cout << line[i].x << " " << line[i].y << endl;
                    if (given_room->cams[j].poz.x == line[i].x
                            && given_room->cams[j].poz.y == line[i].y)
                        drawCam(j);

                }*/
        }
    }
}

void editorscene::EraseWalls(QVector <tchk> line)
{
    QBrush whiteBrush (Qt::white);
    QBrush blackBrush(Qt::black);
    QPen blackPen (Qt::black);
    blackPen.setWidth(2);
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i].x > 0 && line[i].y > 0 && line[i].x < given_room->cam_map[0].size()-1 && line[i].y < given_room->cam_map.size()-1)
        {
            if (given_room->cam_map[line[i].y][line[i].x] == -1 || given_room->cam_map[line[i].y][line[i].x] > 0)
            {
                given_room->cam_map[line[i].y][line[i].x] = 0;
                /*rect = scene->addRect(20*line[i].x,20*line[i].y,20,20,blackPen,whiteBrush);
                for (int j = 0; j < given_room->cams.size(); j++)
                {
                    if (given_room->cams[j].poz.x == line[i].x
                            && given_room->cams[j].poz.y == line[i].y)
                        drawCam(j);

                }*/
            }
        }
    }
}


void editorscene::drawCam(int i)
{
    cout << "povorot: " << given_room->cams[i].povorot << endl;
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(20*given_room->cams[i].poz.x+2, 20*given_room->cams[i].poz.y+2, 16, 16);

    qint32 right_side = given_room->cams[i].povorot+given_room->cams[i].angle;
    if (right_side <= 90)
    {
        item->setStartAngle((90-right_side)*16);
    }
    else
    {
        item->setStartAngle((450-right_side)*16);
    }
    item->setSpanAngle(given_room->cams[i].angle*16);
    item->setBrush(QBrush(Qt::red));
    scene->addItem(item);
}

void editorscene::SetCam(int index, tchk coord)
{
    given_room->cams[index].poz.x = coord.x;
    given_room->cams[index].poz.y = coord.y;
    //drawCam(index);
}

/*void editorscene::removeCameraFromEditor(int index)
{
    QBrush whiteBrush (Qt::white);
    QBrush blackBrush(Qt::black);
    QPen blackPen (Qt::black);
    blackPen.setWidth(2);
    QGraphicsRectItem *rect;
    if (given_room->cam_map[given_room->cams[index].poz.y][given_room->cams[index].poz.x])
    {
        cout << 3 << endl;
        rect = scene->addRect(20*given_room->cams[index].poz.x,
                20*given_room->cams[index].poz.y,20,20,blackPen,blackBrush);
    }
    else
        rect = scene->addRect(20*given_room->cams[index].poz.x,
                20*given_room->cams[index].poz.y,20,20,blackPen,whiteBrush);
    for (int i = 0; i < given_room->cams.size(); i++)
    {
        cout << 5 << endl;
        if (given_room->cams[i].poz.x == given_room->cams[index].poz.x
                && given_room->cams[i].poz.y == given_room->cams[index].poz.y && i != index)
            drawCam(i);
    }
}*/

void editorscene::setPriority(QVector <tchk> line, int value)
{
    QBrush whiteBrush (Qt::white);
    QBrush blackBrush(Qt::black);
    QPen blackPen (Qt::black);
    blackPen.setWidth(2);
    for (int i = 0; i < line.size(); i++)
    {
        cout << "Line " << i << " " << line[i].x << " " << line[i].y << endl;
        if (line[i].x > 0 && line[i].y > 0 && line[i].x < given_room->cam_map[0].size()-1 && line[i].y < given_room->cam_map.size()-1)
        {
            if (given_room->cam_map[line[i].y][line[i].x] == 0 || given_room->cam_map[line[i].y][line[i].x] < -1)
            {
                //cout << 1 << end
                   given_room->cam_map[line[i].y][line[i].x] = (-1)*(value);
            }
        }
    }
}

void editorscene::mouseReleaseEvent(QMouseEvent *ev)
{
    cout << "Released" << endl;
    for (int i = 0; i < given_room->cams.size(); i++)
    {
        cout << i << " " << given_room->cams[i].poz.x << " " << given_room->cams[i].poz.x << endl;
    }

    QPointF cur;
    cur = ev->pos();
    this->end.x = (cur.x()+1)/20;
    this->end.y = (cur.y()+1)/20;
    cout << "End: " << end.x << " " << end.y << endl;

    if (mainWindowUi->CurrentToolLabel->text() == "Current Tool: Wall Brush")
    {
        QVector <tchk> line;
        GetLine(start, end, line);
        DrawWalls(line, mainWindowUi->heightPartWallSpinBox->value());
        cout << "drew walls" << endl;
    }
    else
        if (mainWindowUi->CurrentToolLabel->text() == "Current Tool: Eraser of Walls")
        {
            QVector <tchk> line;
            GetLine(start, end, line);
            EraseWalls(line);
        }
        else
            if (mainWindowUi->CurrentToolLabel->text() == "Current Tool: Camera Brush" && (given_room->cam_map[end.y][end.x] == 0 || given_room->cam_map[end.y][end.x] < -1))
            {
                if (mainWindowUi->camerasListWidget->selectedItems().size() == 0)
                {
                    QMessageBox::information(this, "Camera selection error", "To use this tool select any camera");
                    return;
                }

                int selectedIndex = mainWindowUi->camerasListWidget->row(mainWindowUi->camerasListWidget->selectedItems()[0]);
                given_room->cams[selectedIndex].active = true;
                cout << "selected index: " << selectedIndex << endl;
                cout << 1 << endl;
                /*if (given_room->cams[selectedIndex].camIsArranged)
                {
                    cout << 2 << endl;
                    QBrush whiteBrush (Qt::white);
                    QBrush blackBrush(Qt::black);
                    QPen blackPen (Qt::black);
                    blackPen.setWidth(2);
                    QGraphicsRectItem *rect;
                    if (given_room->cam_map[given_room->cams[selectedIndex].poz.y][given_room->cams[selectedIndex].poz.x])
                    {
                        cout << 3 << endl;
                        rect = scene->addRect(20*given_room->cams[selectedIndex].poz.x,
                                20*given_room->cams[selectedIndex].poz.y,20,20,blackPen,blackBrush);
                    }
                    else
                        rect = scene->addRect(20*given_room->cams[selectedIndex].poz.x,
                                20*given_room->cams[selectedIndex].poz.y,20,20,blackPen,whiteBrush);
                    cout << 4 << endl;
                    for (int i = 0; i < given_room->cams.size(); i++)
                    {
                        cout << 5 << endl;
                        if (given_room->cams[i].poz.x == given_room->cams[selectedIndex].poz.x
                                && given_room->cams[i].poz.y == given_room->cams[selectedIndex].poz.y && i != selectedIndex)
                            drawCam(i);

                    }
                }*/
                SetCam(selectedIndex, end);
                //drawCam(selectedIndex);
            }
            else
                if (mainWindowUi->CurrentToolLabel->text() == "Current Tool: Priority Brush")
                {
                    QVector <tchk> line;
                    GetLine(start, end, line);
                    mainWindowUi->prioritySpinBox->setValue(mainWindowUi->prioritySpinBox->value()-mainWindowUi->prioritySpinBox->value()%5);
                    for (int i = 0; i < line.size(); i++)
                        cout << line[i].x << " " << line[i].y << endl;

                    cout << "value " << mainWindowUi->prioritySpinBox->value() << endl;
                    setPriority(line, mainWindowUi->prioritySpinBox->value());
                }
    double benefit = given_room->count_cost(given_room->cams, false);
    cout << benefit << endl;
    /*for (int i = 0; i < given_room->cam_map.size(); i++)
    {
        for(int j = 0; j < given_room->cam_map[0].size(); j++)
        {
            if(given_room->cam_map[i][j] < -1)
            {
                benefit -= given_room->cam_map[i][j];
            }
        }
    }*/

    mainWindowUi->statusbar->showMessage("Benefit is: " + QString::number(benefit));
    cout << "showed message" << endl;
    visualization(given_room, scene);

}



void editorscene::mousePressEvent(QMouseEvent *ev)
{
   cout << " Pressed " << endl;
   QPointF cur;
   cur = ev->pos();
   this->start.x = (cur.x()+1)/20;
   this->start.y = (cur.y()+1)/20;
   //mainWindowUi->StartPosLabel->setText(QString("Start Pos: \t X = %1, Y = %2").arg(this->start.x).arg(this->start.y));

   // emit Mouse_Pressed();
}
