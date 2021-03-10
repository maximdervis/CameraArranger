#ifndef ROOM_H
#define ROOM_H

#include <QMainWindow>
#include <QVector>
#include <iostream>
#include <QDebug>
#include <QPair>
#include <QThread>

#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <utility>

using namespace std;

namespace Ui {
class room;
}

struct tchk{
    int x,y;
};
struct cam{
        tchk poz; //position
        int povorot; //0-359    rotation angle
        int angle; //0-359  viewing angle
        int size_of_view;
        bool active;
 };

class room : public QMainWindow{
    Q_OBJECT;
public:
    //creation of room
    room();
    explicit room(int size_x, int size_y, int size_z, QVector<QVector<int> > map_of_room, int number_of_cams, QVector <cam> v_cams);

    void set_room(int size_x, int size_y, QVector<QVector<bool> > map_of_room);

    ///КАКИМ ОБРАЗОМ ЗАДАВАТЬ КАМЕРЫ
        /*poz.x и poz.y можно у всех камер одинаковые, но эти точки должны быть ВНУТРИ здания.
        Желательно, чтобы из любой комнаты здания можно было попасть в любую движениями.
        angle ставим как угол обзора камеры
        size_of_view
        Представим, что камера смотрит лучом горизонтально.
        Сюда передаём int(!) значение(лучше округлённое вниз, если оно дробное), в котором пишем на сколько камера максимально может видеть.
        (в идеале прикидывать на каком максимальном расстоянии будет различимо лицо)
        СТАВИМ НЕ БОЛЬШЕ 1000000*/

    void set_cams(QVector<cam> cameras);
    int getMaxSizeOfView();
    void find_best_cams(int populations, double& benefit);

    //result output
    void print_room();

    QVector<cam> get_cams_list(bool do_print);
    QVector < QVector<double> > getBenefitMap();
    double count_cost(QVector <cam> a, bool print_room_for_debug);
    bool EmptyCell(const QVector<QVector<double>>& cost_map, const tchk cell, const tchk nextCell);
    QVector < QVector<int> > cam_map; //walls of a room: 0-empty (-1)-wall l - part wall (l - height)
    QVector <cam> cams; //cameras
    int height;

private:



    cam mutac(cam c);

    QVector<cam> mutac_mas(QVector<cam> a);

    void change_cost(QPair<QVector<cam>,double> *a);



    //values
private:
    int N_OBJ = 8*9;
    double pi = 3.1415926535;
    int width, length;////width and length

    int n_cams;
    QVector < QVector<double> > benefit_map;//benefit

};

#endif // ROOM_H
