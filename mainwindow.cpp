#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QtXml>
#include <QCoreApplication>


#include "cameraaddition.h"
#include "dbcameraaddition.h"
#include "roomcreation.h"
#include "camerainformation.h"
#include "settings.h"
#include "editorscene.h"
#include "room.h"

#include <QMessageBox>
#include <QCheckBox>
#include <QListWidgetItem>

#include <QLabel>
#include <QtXml>

#include <QFileDialog>
#include <fstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    populationsAmnt = 300;

    curBenefit = -1;

    editorCellSize = 20;
    editorLength = 10;
    editorWidth = 10;
    editorHeight = 10;
    roomIsCreated = false;
    arrangementFound = false;
    sceneIsCreated = false;

    QPixmap pixmap(":/tool/resources/wall.png");
    QIcon ButtonIcon(pixmap);
    ui->WallButton->setIcon(ButtonIcon);
    ui->WallButton->setIconSize(pixmap.rect().size());
    QPixmap pixmap2(":/tool/resources/eraser.png");
    QIcon ButtonIcon2(pixmap2);
    ui->EraseButton->setIcon(ButtonIcon2);
    ui->EraseButton->setIconSize(pixmap2.rect().size());
    QPixmap pixmap3(":/tool/resources/camera.png");
    QIcon ButtonIcon3(pixmap3);
    ui->CameraButton->setIcon(ButtonIcon3);
    ui->CameraButton->setIconSize(pixmap3.rect().size());
    QPixmap pixmap4(":/tool/resources/priority.png");
    QIcon ButtonIcon4(pixmap4);
    ui->PriorityButton->setIcon(ButtonIcon4);
    ui->PriorityButton->setIconSize(pixmap4.rect().size());

    ui->rotationSpinBox->setRange(0,359);
    ui->rotationSpinBox->setSingleStep(5);

    ui->heightPartWallSpinBox->setRange(1, editorHeight);

    ui->prioritySpinBox->setRange(0,100);
    ui->prioritySpinBox->setSingleStep(5);

    FillNameToSpecsMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//processing clicking on create new camera button
void MainWindow::on_addCameraButton_clicked()
{
    //creating a new variable that stores new CameraAddition window
    CameraAddition cameraAdditionWindow;

    //removing what's this and close button from QDialog window
    cameraAdditionWindow.setWindowFlags(cameraAdditionWindow.windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowCloseButtonHint);
    //making new window modal (we can't use parent window while using this one)
    cameraAdditionWindow.setModal(true);

    QFile camerasBdFile(QApplication::applicationDirPath() + "/cameras.xml");
    QString cameraTag = "Camera";
    QString nameAttribute = "name";
    if(!cameraAdditionWindow.updateDBCamerasListFromXmlFile(&camerasBdFile, cameraTag, nameAttribute))
    {
        QMessageBox::warning(this, "Reading cameras error", "It seems that the list with cameras is empty\nadd a new camera");
    }

    //opening just created window
    cameraAdditionWindow.exec();

    if(cameraAdditionWindow.getAdditionStatus())
    {
        //getting added camera name
        QString addedCameraName = cameraAdditionWindow.getCameraName();
        int camerasAmount = cameraAdditionWindow.getCamerasAmount();

        //adding chosen amount of cameras to list of cameras in room
        for(int i = 0; i < camerasAmount; i++)
        {
            ui->camerasListWidget->addItem(addedCameraName);
        }
        FillNameToSpecsMap();

        cam cur;
        if(getEditorWidth() > 3)
            cur.poz.x = rand()%(getEditorWidth() - 2) + 1;
        else
            cur.poz.x = 1;

        if(getEditorLength() > 3)
            cur.poz.y = rand()%(getEditorLength() - 2) + 1;
        else
            cur.poz.y = 1;

        cur.povorot = (((cur.povorot + rand() % 180 - 90 +360)%360)/5) *5;;
        cur.angle = nameToSpecs[addedCameraName].angle;
        cur.size_of_view = nameToSpecs[addedCameraName].size_of_view;
        cur.active = false;

        for (int i = 0; i < camerasAmount; i++)
        {
            if(getEditorWidth() > 3)
                cur.poz.x = rand()%(getEditorWidth() - 2) + 1;
            else
                cur.poz.x = 1;

            if(getEditorLength() > 3)
                cur.poz.y = rand()%(getEditorLength() - 2) + 1;
            else
                cur.poz.y = 1;
            v_cams.push_back(cur);
            if (roomIsCreated)
                given_room->cams.push_back(cur);
        }
        if (sceneIsCreated)
        {
            double benefit = graphicsView->given_room->count_cost(given_room->cams, false);

            ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));
            //given_room->count_cost(given_room->cams, false);
            //graphicsView->visualization(given_room, scene);
            graphicsView->visualization(graphicsView->given_room, scene);
        }
    }
}

void MainWindow::on_removeCameraButton_clicked()
{
    //removing all selected cameras
    //getting selected index
    if (ui->camerasListWidget->selectedItems().size() == 0)
    {
        QMessageBox::warning(this, "Camera selection error", "To use this tool select any camera");
        return;
    }
    int selectedIndex = ui->camerasListWidget->row(ui->camerasListWidget->selectedItems()[0]);
    cout << "selectedIndex removing: " << selectedIndex << endl;
    cout << 1 << endl;
    /*if (sceneIsCreated)
        graphicsView->removeCameraFromEditor(selectedIndex);
    cout << 2 << endl;*/
    v_cams.erase(v_cams.begin() + selectedIndex);

    if (roomIsCreated)
    {
        given_room->cams.erase(given_room->cams.begin() + selectedIndex);
        double benefit = given_room->count_cost(given_room->cams, false);

        ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));
        graphicsView->visualization(given_room, scene);
    }

    qDeleteAll(ui->camerasListWidget->selectedItems());

}

void MainWindow::setupRoomGrid()
{
    QBrush blackBrush (Qt::black);
    QBrush whiteBrush (Qt::white);

    QPen blackPen (Qt::black);
    blackPen.setWidth(2);

    rect = scene->addRect(0,0,20*(editorWidth)-4,20,blackPen,blackBrush);
    for (qint32 i = 1; i < editorLength-1; i++)
    {
        rect = scene->addRect(0,20*i,20,20,blackPen,blackBrush);
        for (qint32 j = 1; j < editorWidth-1; j++)
        {
            rect = scene->addRect(20*j,20*i,20,20,blackPen,whiteBrush);
        }
        rect = scene->addRect(20*(editorWidth-1),20*i,16,20,blackPen,blackBrush);
    }
    rect = scene->addRect(0,20*(editorLength-1),20*(editorWidth)-4,16,blackPen,blackBrush);

}


void MainWindow::on_actionNew_room_triggered()
{
    //creating a new variable that stores new RoomCreation window
    RoomCreation roomCreationWindow;
    //making new window modal (we can't use parent window while using this one)
    roomCreationWindow.setModal(true);
    //removing what's this and close button from QDialog window
    roomCreationWindow.setWindowFlags(roomCreationWindow.windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowCloseButtonHint);
    //opening just created window
    roomCreationWindow.exec();

    //if confirm creation button pressed
    if(roomCreationWindow.getCreationStatus())
    {
        //get and set data about room
        editorWidth = roomCreationWindow.getEnteredWidth();
        editorLength = roomCreationWindow.getEnteredLength();
        editorHeight = roomCreationWindow.getEnteredHeight();
        editorWidth += 2;
        editorLength += 2;
        ui->heightPartWallSpinBox->setRange(1, editorHeight);
        ui->heightPartWallSpinBox->setValue(editorHeight);

        for (int i = 0; i < v_cams.size(); i++)
        {
            if(getEditorWidth() > 3)
                v_cams[i].poz.x = rand()%(getEditorWidth() - 2) + 1;
            else
                v_cams[i].poz.x = 1;

            if(getEditorLength() > 3)
                v_cams[i].poz.y = rand()%(getEditorLength() - 2) + 1;
            else
                v_cams[i].poz.y = 1;
        }

        map_of_room = QVector<QVector<int>> (editorLength, QVector<int>(editorWidth));
        for (int i = 0; i < map_of_room[0].size(); i++)
        {
            map_of_room[0][i] = -1;
        }
        for (int j = 1; j < map_of_room.size()-1; j++)
        {
            map_of_room[j][0] = -1;
            map_of_room[j][map_of_room[0].size()-1] = -1;
        }
        for (int i = 0; i < map_of_room[0].size(); i++)
        {
            map_of_room[map_of_room.size()-1][i] = -1;
        }

        given_room = new room(editorWidth, editorLength, editorHeight, map_of_room, v_cams.size(), v_cams);

        //removing previous room from editor
        clearRoomEditor();

        graphicsView = new editorscene(this, ui, given_room);
        ui->gridScrollArea->setWidget(graphicsView);

        ui->heightPartWallSpinBox->setRange(1, editorHeight);

        scene = new QGraphicsScene(this);
        sceneIsCreated = true;
        graphicsView->addScene(scene);
        graphicsView->setScene(scene);

        graphicsView->setFixedSize(20*editorWidth, 20*editorLength);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //setupRoomGrid();

        /*for (int i = 0; i < v_cams.size(); i++)
        {
            graphicsView->drawCam(i);
        }*/

        double benefit = given_room->count_cost(given_room->cams, false);


        ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));

        graphicsView->visualization(given_room, scene);

        roomIsCreated = true;
    }
}


//removing editor
void MainWindow::clearRoomEditor()
{
    if (sceneIsCreated)
    {
        delete graphicsView;
        delete scene;
    }
    sceneIsCreated = false;
}

qint32 MainWindow::getEditorLength()
{
    return editorLength;
}

qint32 MainWindow::getEditorWidth()
{
    return editorWidth;
}

void MainWindow::on_actionRun_triggered()
{
    if(!roomIsCreated)
    {
        QMessageBox::warning(this, "Room is not initialized", "You must create a room before running\npress Actions->new room");
        return;
    }else
    if(ui->camerasListWidget->count() == 0)
    {
        QMessageBox::warning(this, "There are no cameras", "You must add at least one camera to room");
        return;
    }

    cout << "Before:\n";
    given_room->print_room();
    cout << endl;

    double benefit = 0;
    given_room->find_best_cams(populationsAmnt, benefit);
    cout << "After:\n";
    given_room->print_room();
    v_cams = given_room->get_cams_list(true);



    QVector < QVector<double> > benefit_map = given_room->getBenefitMap();

    graphicsView->visualization(given_room, scene);
    ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));
    arrangementFound = true;
}

void MainWindow::FillNameToSpecsMap()
{

    //getting cameras from xml-file
    //create a new document
    QDomDocument document("cameras");
    QFile file(QCoreApplication::applicationDirPath() + "/cameras.xml");

    //try to open a new file
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Cameras list is empty", "There was no file with cameras\nadd a new camera to list");
    }else
    {
        //fill document with data from file
        if(!document.setContent(&file))
        {
            //close file and return false because nothing's done
            file.close();
        }
        //close file
        file.close();
    }
    //update cameras in list
    QDomElement camerasRoot = document.firstChildElement();

    QDomNodeList items = camerasRoot.elementsByTagName("Camera");
    //iterating over all found elements with such a tag
    for(int i = 0; i < items.length(); i++)
    {

        QDomNode currentItemNode = items.at(i);
        if(currentItemNode.isElement())
        {
            //converting item node to element
            QDomElement currentItemElemenet = currentItemNode.toElement();
            //getting name of each item
            QString currentItemName = currentItemElemenet.attribute("name");

            //adding each item's name and specs to map
            qint32 currentItemMaxViewingAngle = currentItemElemenet.attribute("viewing_angle").toInt();
            qint32 currentItemSizeOfView = currentItemElemenet.attribute("visibility_range").toInt();
            nameToSpecs[currentItemName].angle = currentItemMaxViewingAngle;
            nameToSpecs[currentItemName].size_of_view = currentItemSizeOfView;

        }

    }

}

void MainWindow::on_camerasListWidget_itemDoubleClicked(QListWidgetItem *item)
{
        //getting information about cameras

        //getting index of selected item
        int clickedItemIndex = ui->camerasListWidget->row(item);
        qDebug()<<"ClICKED INDEX: "<<clickedItemIndex;

        //creating a new variable that stores new RoomCreation window
        CameraInformation cameraInformationWindow(editorWidth, editorLength);
        //making new window modal (we can't use parent window while using this one)
        cameraInformationWindow.setModal(true);
        //removing what's this and close button from QDialog window
        cameraInformationWindow.setWindowFlags(cameraInformationWindow.windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowCloseButtonHint);

        if (!roomIsCreated)
        {
            QMessageBox::warning(this, "Room is not created", "You should create room to view camera information");
            return;
        }
        v_cams = given_room->get_cams_list(true);
        //passing info
        cameraInformationWindow.setName(item->text());
        cameraInformationWindow.setViewingAngle(v_cams[clickedItemIndex].angle);
        cameraInformationWindow.setRotationAngle(v_cams[clickedItemIndex].povorot);
        cameraInformationWindow.setPositionX(v_cams[clickedItemIndex].poz.x);
        cameraInformationWindow.setPositionY(v_cams[clickedItemIndex].poz.y);
        cameraInformationWindow.setVisibilityRange(v_cams[clickedItemIndex].size_of_view);
        cameraInformationWindow.setupInfo();


        //opening just created window
        cameraInformationWindow.exec();

        //getting info back
        v_cams[clickedItemIndex].angle = cameraInformationWindow.getViewingAngle();
        v_cams[clickedItemIndex].povorot = cameraInformationWindow.getRotationAngle();
        v_cams[clickedItemIndex].poz.x = cameraInformationWindow.getPositionX();
        v_cams[clickedItemIndex].poz.y = cameraInformationWindow.getPositionY();
        v_cams[clickedItemIndex].size_of_view = cameraInformationWindow.getVisibilityRange();
        if (roomIsCreated)
        {
            given_room->set_cams(v_cams);//сделать не криво
            if (sceneIsCreated)
            {
                double benefit = given_room->count_cost(given_room->cams, false);

                ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));
                graphicsView->visualization(given_room, scene);
            }
        }
}




void MainWindow::on_actionSettings_triggered()
{
    //creating a new variable that stores new Settings window
    Settings settingsWindow;
    //making new window modal (we can't use parent window while using this one)
    settingsWindow.setModal(true);
    //removing what's this and close button from QDialog window
    settingsWindow.setWindowFlags(settingsWindow.windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowCloseButtonHint);

    settingsWindow.exec();

   if(settingsWindow.getSettingsChangingStatus())
   {
       populationsAmnt = settingsWindow.getPopulationsAmnt();
   }
}

void MainWindow::on_WallButton_clicked()
{
    ui->CurrentToolLabel->setText(QString ("Current Tool: Wall Brush"));
    ui->heightPartWallSpinBox->setValue(editorHeight);

    /*QPixmap pixmap(":/tool/resources/wall.png");
    ui->toolPict->setPixmap(pixmap);*/

    //QIcon ButtonIcon4(pixmap4);
    //ui->PriorityButton->setIcon(ButtonIcon4);
    //ui->PriorityButton->setIconSize(pixmap4.rect().size());
}

void MainWindow::on_EraseButton_clicked()
{
    ui->CurrentToolLabel->setText(QString ("Current Tool: Eraser of Walls"));
    /*QPixmap pixmap(":/tool/resources/eraser.png");
    ui->toolPict->setPixmap(pixmap);*/
}

void MainWindow::on_CameraButton_clicked()
{
    /*for (int i = 0; i < given_room->cams.size(); i++)
    {
        if (given_room->cams[i].active)
            given_room->cams[i].active = false;
    }
    int selectedIndex = ui->camerasListWidget->row(ui->camerasListWidget->selectedItems()[0]);
    given_room->cams[selectedIndex].active = true;*/
    graphicsView->visualization(given_room, scene);
    ui->CurrentToolLabel->setText(QString ("Current Tool: Camera Brush"));
    /*QPixmap pixmap(":/tool/resources/camera.png");
    ui->toolPict->setPixmap(pixmap);*/
}

void MainWindow::on_PriorityButton_clicked()
{
    ui->CurrentToolLabel->setText(QString ("Current Tool: Priority Brush"));
    /*QPixmap pixmap(":/tool/resources/priority.png");
    ui->toolPict->setPixmap(pixmap);
    ui->toolPict->setFixedWidth(ui->gridScrollArea->width());*/
}

void MainWindow::on_rotationSpinBox_valueChanged(int arg1)
{
    if (ui->rotationSpinBox->value() == 0)
        return;
    if (!(ui->CurrentToolLabel->text() == "Current Tool: Camera Brush"))
    {
        ui->rotationSpinBox->setValue(0);
        QMessageBox::warning(this, "Tool selection error", "To change angle choose Camera Brush");
        return;
    }
    if (ui->camerasListWidget->selectedItems().size() == 0)
    {
        QMessageBox::warning(this, "Camera selection error", "To use this tool select any camera");
        ui->rotationSpinBox->setValue(0);
        return;
    }
    int selectedIndex = ui->camerasListWidget->row(ui->camerasListWidget->selectedItems()[0]);
    if (!roomIsCreated)
    {
        QMessageBox::warning(this, "Room is not initialized", "You must create a room and arrange camera before changing the angle");
        ui->rotationSpinBox->setValue(0);
        return;
    }
    /*if (!given_room->cams[selectedIndex].camIsArranged)
    {
        QMessageBox::warning(this, "Camera is not arranged", "You must arrange camera before changing the angle");
        ui->rotationSpinBox->setValue(0);
        return;
    }*/


    given_room->cams[selectedIndex].povorot = ui->rotationSpinBox->value();

    double benefit = given_room->count_cost(given_room->cams, false);

    ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));
    graphicsView->visualization(given_room, scene);
    /*QBrush whiteBrush (Qt::white);
    QBrush blackBrush(Qt::black);
    QPen blackPen (Qt::black);
    blackPen.setWidth(2);
    QGraphicsRectItem *rect;

    if (given_room->cam_map[given_room->cams[selectedIndex].poz.x][given_room->cams[selectedIndex].poz.y])
    {
        rect = scene->addRect(20*given_room->cams[selectedIndex].poz.x,
                20*given_room->cams[selectedIndex].poz.y,20,20,blackPen,blackBrush);
    }
    else
        rect = scene->addRect(20*given_room->cams[selectedIndex].poz.x,
                20*given_room->cams[selectedIndex].poz.y,20,20,blackPen,whiteBrush);
    for (int i = 0; i < given_room->cams.size(); i++)
    {
        if (given_room->cams[i].poz.x == given_room->cams[selectedIndex].poz.x
                && given_room->cams[i].poz.y == given_room->cams[selectedIndex].poz.y)
            graphicsView->drawCam(i);

    }
    cout << "angle: " << given_room->cams[selectedIndex].povorot << endl;
    graphicsView->drawCam(selectedIndex);*/
}

void MainWindow::on_actionSave_as_triggered()
{
    if(!roomIsCreated)
    {
        QMessageBox::warning(this, "Room does not exist", "You should create or open room before saving");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save location", "C://", "All File (*.txt)");
    qDebug()<<filePath;
    QFile roomFile(filePath);
    if(!roomFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Room file saving error", "Cannot create new file");
        return;
    }else
    {

        QTextStream out(&roomFile);

        out << graphicsView->given_room->cam_map.length() << " ";
        out << graphicsView->given_room->cam_map[0].length() << endl;
        out << given_room->height << endl;

        for(int i = 0; i < graphicsView->given_room->cam_map.length(); i++)
        {
            for(int j = 0; j < graphicsView->given_room->cam_map[i].length(); j++)
                out << graphicsView->given_room->cam_map [i][j] << " " ;
            out<< endl;
        }

        v_cams = given_room->get_cams_list(true);
        out << endl;
        out<<v_cams.size()<<endl;
        for(int i = 0; i < v_cams.size(); i++)
        {
            out<<ui->camerasListWidget->item(i)->text()<<" ";
            out<<v_cams[i].poz.x<<" "<<v_cams[i].poz.y<<" ";
            out<<v_cams[i].angle<<" ";
            out<<v_cams[i].povorot<<" ";
            out<<v_cams[i].size_of_view;
            out<<endl;
        }
        roomFile.flush();
        roomFile.close();
    }

}

void MainWindow::on_actionOpen_room_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Save location", "C://", "All File (*.txt)");
    qDebug()<<filePath;
    QFile roomFile(filePath);
    if(!roomFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Room opening error", "Cannot open file");
        return;
    }else
    {
        QTextStream in(&roomFile);

        in >> editorLength;
        in >> editorWidth;
        in >> editorHeight;
        try
        {
            map_of_room = QVector<QVector<int>> (editorLength, QVector<int>(editorWidth));

            for(int i = 0; i < map_of_room.size(); i++)
            {
                for(int j = 0; j < map_of_room[0].size(); j++)
                {
                    qint32 cellInfo;
                    in>>cellInfo;
                    map_of_room[i][j] = cellInfo;
                }
            }

            QString camsAmnt;
            in >> camsAmnt;

            v_cams = QVector<cam> (camsAmnt.toInt());
            ui->camerasListWidget->clear();
            for(int i = 0; i < v_cams.size(); i++)
            {
                QString cameraName;
                in>>cameraName;
                ui->camerasListWidget->addItem(cameraName);
                //qDebug()<<cameraName<<endl;
                in>>v_cams[i].poz.x>>v_cams[i].poz.y;
                in>>v_cams[i].angle;
                in>>v_cams[i].povorot;
                in>>v_cams[i].size_of_view;
                v_cams[i].active = false;
            }

            cout << 1 << endl;
            given_room = new room(editorWidth, editorLength, editorHeight, map_of_room, v_cams.size(), v_cams);
            cout << 2 << endl;
            clearRoomEditor();

            graphicsView = new editorscene(this, ui, given_room);
            ui->gridScrollArea->setWidget(graphicsView);

            scene = new QGraphicsScene(this);
            sceneIsCreated = true;
            graphicsView->addScene(scene);
            graphicsView->setScene(scene);

            graphicsView->setFixedSize(20*editorWidth, 20*editorLength);
            graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            double benefit = given_room->count_cost(given_room->cams, false);

            ui->statusbar->showMessage("Benefit is: " + QString::number(benefit));
            graphicsView->visualization(given_room, scene);

            roomIsCreated = true;
            roomFile.flush();
            roomFile.close();

            //graphicsView->visualization(given_room,graphicsView->scene);
            cout << 5 << endl;

        }  catch (exception ex)
        {
            QMessageBox::warning(this, "Incorrect data", ex.what());
        }

    }


}

void MainWindow::on_camerasListWidget_itemClicked(QListWidgetItem *item)
{
    if (!roomIsCreated)
        return;
    for (int i = 0; i < given_room->cams.size(); i++)
    {
        if (given_room->cams[i].active)
            given_room->cams[i].active = false;
    }
    int selectedIndex = ui->camerasListWidget->row(ui->camerasListWidget->selectedItems()[0]);
    given_room->cams[selectedIndex].active = true;
    graphicsView->visualization(given_room, scene);
    /*int selectedIndex = ui->camerasListWidget->row(ui->camerasListWidget->selectedItems()[0]);

    if (roomIsCreated && sceneIsCreated)
    {
        QBrush transparentBrush(QColor(0,0,0,0));
        QPen bluePen(Qt::blue);
        bluePen.setWidth(4);
        graphicsView->scene->addRect(20*given_room->cams[selectedIndex].poz.x,
                                     20*given_room->cams[selectedIndex].poz.y,20,20,bluePen,transparentBrush);
    }*/
}


