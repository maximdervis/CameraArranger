#include "room.h"
#include <QDebug>
#include <QObject>

bool comp(QPair<QVector<cam>,double> a, QPair<QVector<cam>,double> b){
    return ((a.second) < (b.second));
}

room:: room(){}

room::room(int size_x, int size_y, int size_z, QVector<QVector<int> > map_of_room, int number_of_cams, QVector <cam> v_cams){
    if(size_x <= 0 || size_y <= 0){
         throw "Non-positive size of room";
    }
    if(number_of_cams < 0){
         throw "Non-positive number of cameras";
    }
    if(map_of_room.size() == 0){
         throw "Void room";
    }
    if(map_of_room.size() != size_y || map_of_room[0].size() != size_x){
         throw "Wrong size of room";
    }
    width = size_x;
    length = size_y;
    height = size_z;
    cam_map = map_of_room;
    n_cams = number_of_cams;
    cams = v_cams;
}

void room::set_cams(QVector<cam> cameras){
    if(cameras.size() == 0){
        throw "No cameras";
    }
    for(int i = 0; i < cameras.size(); i++){
        if(cameras[i].povorot < 0 || cameras[i].povorot > 360){
            throw "Wrong parameter: povorot";
        }
        if(cameras[i].angle <= 0 || cameras[i].angle > 360){
            throw "Wrong parameter: angle";
        }
        if(cameras[i].size_of_view <= 0 || cameras[i].size_of_view >= 1000000){
            throw "Wrong parameter: size_of_view";
        }
    }
    n_cams = cameras.size();
    cams = cameras;
}

int room::getMaxSizeOfView()
{
    int max_size_of_view = 0;
    //finding max size of view of cameras
    for(int i = 0; i < cams.size(); i++)
    {
        if (cams[i].size_of_view > max_size_of_view)
            max_size_of_view = cams[i].size_of_view;
    }
    return max_size_of_view;
}

void room::find_best_cams(int populations, double &benefit){
    int num_of_elements_in_population_divide_8 = 3;
    if(populations <= 0){
        throw "Wrong count of populations";
    }
    if(num_of_elements_in_population_divide_8 <= 0){
        throw "Wrong parameter: num_of_elements_in_population_divide_8";
    }
    if(cams.size() == 0){
        throw "No cameras";
    }
    n_cams = cams.size();
    if(cams.size() != n_cams){
        throw "Wrong cameras";
    }

    num_of_elements_in_population_divide_8 *= 8;
    //array with data of individuals in pairs, location of cameras / cost
    QVector < QPair<QVector<cam>,double> > pop_mas(num_of_elements_in_population_divide_8);
    for(int i = 0; i < num_of_elements_in_population_divide_8; i++){
        pop_mas[i].first = cams;
        pop_mas[i].second = 0;
    }
    QVector <cam> best_r;
    double best_cost = 0;
    ///Populations
    for(int popl = 1; popl <= populations; popl++){
        ///Count cost
        for(int i = 0; i < pop_mas.size(); i+=1){
            change_cost(&pop_mas[i]);
            pop_mas[i].second = count_cost(pop_mas[i].first,false);
        }
        sort(pop_mas.begin(),pop_mas.end(),comp);

        ///Make next population
        int del = pop_mas.size() / 3;
        //leave n/3 of the best
        //transform n/3 of next
        for(int i = del; i < 2*del; i++){
            pop_mas[i].first = mutac_mas(pop_mas[i].first);
        }
        //n/3 of left ones equate to best ones and transform
        for(int i = 0; i < del; i++){
            pop_mas[i].first = mutac_mas(pop_mas[pop_mas.size()-i-1].first);
        }
        ///Storage
        double now_result = pop_mas[pop_mas.size()-1].second;
        if(now_result > best_cost){
            best_cost = now_result;
            best_r = pop_mas[pop_mas.size()-1].first;
        }
        if(popl%100==0)
            cout << popl << "/" << populations << " " << best_cost << endl;
    }
    cams = best_r;
    benefit = best_cost;
}


void room::print_room(){
    count_cost(cams,true);
}


QVector<cam> room::get_cams_list(bool do_print){
    if(do_print)
        for(auto i: cams){
            cout << i.poz.x << " " << i.poz.y << " " << i.povorot << " " << i.angle << " " << i.size_of_view << '\n';
        }
    return cams;
}


bool room::EmptyCell(const QVector<QVector<double>>& cost_map, const tchk cell, const tchk nextCell)
{
    if(cell.x < 0 || cell.y < 0 ||
       cell.x >= cost_map.size() || cell.y >= cost_map[cell.x].size() ||
       (cell.x < cost_map.size() - 1 && cell.y < cost_map[cell.x].size() - 1 && cost_map[cell.x + 1][cell.y] == 1000001  && cost_map[cell.x][cell.y + 1] == 1000001 && nextCell.x == cell.x + 1 && nextCell.y == cell.y + 1) ||
       (cell.x > 0 && cell.y > 0 && cost_map[cell.x - 1][cell.y] == 1000001 && cost_map[cell.x][cell.y - 1] == 1000001 && nextCell.x == cell.x - 1 && nextCell.y == cell.y - 1) ||
       (cell.x < cost_map.size() - 1 && cell.y > 0 && cost_map[cell.x + 1][cell.y] == 1000001 && cost_map[cell.x][cell.y - 1] == 1000001 && nextCell.x == cell.x + 1 && nextCell.y == cell.y - 1) ||
       (cell.x > 0 && cell.y < cost_map[cell.x].size() - 1 && cost_map[cell.x - 1][cell.y] == 1000001 && cost_map[cell.x][cell.y + 1] == 1000001 && nextCell.x == cell.x - 1 && nextCell.y == cell.y + 1))
        return false;

    return cost_map[cell.x][cell.y] != 1000001;
}


double room::count_cost(QVector <cam> a, bool print_room_for_debug = false){
        ///делаем массив стоимостей(1000001 = стена, 1000010+l = препятствие высотой l)
        double rs = 0;
        for (int i = 0; i < cam_map.size(); i++)
        {
            for(int j = 0; j < cam_map[0].size(); j++)
            {
                if(cam_map[i][j] < -1)
                {
                    rs -= cam_map[i][j];
                }
            }
        }

        QVector < QVector<double> > cost_map(length);
        for(int i = 0; i < length; i++){
            for(int j = 0; j < width; j++){
                    if(cam_map[i][j] == -1){
                        cost_map[i].push_back(1000001);
                    }else if(cam_map[i][j] <= 0){
                        cost_map[i].push_back(cam_map[i][j]);
                    }else if(cam_map[i][j] >= height){
                        cost_map[i].push_back(1000001);
                    }else{
                        cost_map[i].push_back(1000010+cam_map[i][j]);
                    }
            }
        }
        ///расставляем камеры
        for(int i = 0; i < a.size(); i++){
            int x = a[i].poz.x;
            int y = a[i].poz.y;
            if(cost_map[y][x] < a[i].size_of_view)
                cost_map[y][x] = a[i].size_of_view;
            //каждые 3 градуса проводим лучи
            int prod_l = 0;
            for(int j = a[i].povorot; j != (a[i].povorot+a[i].angle)%360 + 1; j = (j+3)%360){
                prod_l++;
                if(prod_l > a[i].angle/3 + 1){
                    break;
                }
                float can_start_from = 0;
                for(float distance = 0; distance <= a[i].size_of_view; distance+=.5){
                    //считаем сдвиг по осям
                    int dx = round(sin(j*pi/180)*distance);
                   int dy = -round(cos(j*pi/180)*distance);

                   int dx_next = round(sin(j*pi/180)*(distance + .5));
                   int dy_next = -round(cos(j*pi/180)*(distance + .5));

                   if(y+dy < cost_map.size()-1 && x+dx < cost_map[0].size()-1){
                       if(cost_map[y+dy][x+dx] == 1000001 || sqrt(dx*dx+dy*dy) >= a[i].size_of_view){
                           break;
                       }else{
                           if(cost_map[y+dy][x+dx] > 1000001){
                                //препятствие
                                float able_distance = distance + distance*(cost_map[y+dy][x+dx]-1000010)/(height-(cost_map[y+dy][x+dx]-1000010));
                                can_start_from = max(can_start_from,able_distance);
                           }else if(distance >= can_start_from){
                               //fill map
                               if(!EmptyCell(cost_map,  {y + dy, x + dx}, {y + dy_next, x + dx_next}))
                                   break;
                               cost_map[y+dy][x+dx] = max(cost_map[y+dy][x+dx],a[i].size_of_view-sqrt(dx*dx+dy*dy));
                           }

                       }
                   }else{
                       break;
                   }
                }
            }
        }

        ///считаем сумму

        for(int i = 0; i < length; i++){
            for(int j = 0; j < width; j++){
                if(cost_map[i][j] < 1000001)
                    rs += cost_map[i][j];
            }
        }

        if(print_room_for_debug){
            for(int i = 0; i < length; i++){
                for(int j = 0; j < width; j++){
                    if(cost_map[i][j] == 1000001){
                        cout << "\t#";
                        continue;
                    }

                    if(cost_map[i][j] > 1000001){
                        cout << "\t*";
                        continue;
                    }
                    cout << fixed;
                    cout << setprecision(2);
                    cout << "\t" << cost_map[i][j];
                }
                cout << endl;
            }
            cout << "Benefit " << rs << endl;
        }
        benefit_map = cost_map;

        return rs;
    }


cam room::mutac(cam c){
    if(rand() % 2 == 0){
        //change position
        int sz_move = rand()%4 + 1;
        int x = c.poz.x;
        int y = c.poz.y;
        int napravl = rand()%4;
        if(napravl == 0){
            if(y + sz_move < length){
                if(cam_map[y + sz_move][x] == 0 || cam_map[y + sz_move][x] < -1)
                    c.poz.y += sz_move;
                else{
                    while(y + sz_move < length-1 && cam_map[y + sz_move][x]){
                        sz_move++;
                    }
                    if(cam_map[y + sz_move][x] == 0 || cam_map[y + sz_move][x] < -1)
                        c.poz.y += sz_move;
                }
            }
        }else if(napravl == 1){
            if(x + sz_move < width){
                if(cam_map[y][x + sz_move] == 0 || cam_map[y][x+ sz_move] < -1)
                    c.poz.x += sz_move;
                else{
                    while(x + sz_move < width-1 && cam_map[y][x + sz_move]){
                        sz_move++;
                    }
                    if(cam_map[y][x + sz_move] == 0 || cam_map[y][x+ sz_move] < -1)
                        c.poz.x += sz_move;
                }
            }
        }else if(napravl == 2){
            if(y-sz_move >= 0){
                if(cam_map[y - sz_move][x] == 0 || cam_map[y - sz_move][x] < -1)
                    c.poz.y -= sz_move;
                else{
                    while(y - sz_move > 0 && cam_map[y - sz_move][x]){
                        sz_move++;
                    }
                    if(cam_map[y - sz_move][x] == 0 || cam_map[y - sz_move][x] < -1)
                        c.poz.y -= sz_move;
                }
            }
        }else{
            if(x - sz_move >= 0){
                if(cam_map[y][x - sz_move] == 0 || cam_map[y][x - sz_move] < -1)
                    c.poz.x -= sz_move;
                else{
                    while(x - sz_move > 0 && cam_map[y][x - sz_move]){
                            sz_move++;
                        }
                        if(cam_map[y][x - sz_move] == 0 || cam_map[y][x - sz_move] < -1)
                            c.poz.x -= sz_move;
                }
            }
        }
    }
    if(rand() % 2 == 0){
        //change angle
        c.povorot = (((c.povorot + rand() % 60 - 30 +360)%360)/5) *5;
    }


    return c;
}


QVector<cam> room::mutac_mas(QVector<cam> a){
    for(int i = 0; i < a.size(); i++){
        a[i] = mutac(a[i]);
    }
    return a;
}

void room::change_cost(QPair<QVector<cam>,double> *a){
    a->second = count_cost(a->first, false);
}

QVector < QVector<double> > room::getBenefitMap(){
    return benefit_map;
}

