#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parameters.h"

#include "dependencies/json.hpp"

#include "OpenGL/widgets/A2_2Dwidget.h"
#include "OpenGL/layers/A3_2dlayer.h"
#include "solution/a3solution.h"


#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

extern bool global_reguralizer;

using json = nlohmann::json;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mode_group = new QActionGroup(this);
    mode_group->addAction(ui->actionSelect_and_Edit_Links);
    mode_group->addAction(ui->actionSelect_and_Edit_Joints);
    mode_group->addAction(ui->actionSelect_and_Edit_Obstacles);
    mode_group->addAction(ui->actionShowtime);
    mode_group->setExclusive(true);

    params = new Parameters();
    QObject::connect(this->params, SIGNAL(dialog_accepted()), this, SLOT(params_updated()));
    params_updated();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mode_group;
    delete params;
}

void MainWindow::on_actionLoad_triggered()
{

    // Logic for file opening, putting into JSON format
    QString filename = QFileDialog::getOpenFileName(this, tr("Load scene."), "", tr("JSON File (*.json)"));

    if (filename.isEmpty())
        return;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

    QTextStream in(&file);
    QString json_data = in.readAll();
    json j = json::parse(json_data.toStdString());

    A2_2DWidget* w = ui->OpenGL;
    A3_2DLayer* l = dynamic_cast<A3_2DLayer*>(w->m_layers[0]);

    // Deletes existing structures
    for(unsigned int i =0; i<l->m_joints.size(); i++){
        // Note, deleting a joint also deletes any associated links
        delete l->m_joints[i];
        l->m_joints[i] = nullptr;
    }
    l->m_joints.clear();
    l->m_springs.clear();

    // Logic for deserialization

    // Map for efficiently creating links
    std::map<std::string, Joint2D*> id_map;

    for (auto itr = j["joints"].begin(); itr!= j["joints"].end(); itr++){
        Joint2D* joint = new Joint2D(l->m_parent);
        joint->m_id = (*itr)["id"];
        id_map[joint->m_id] = joint;
        joint->set_locked((*itr)["locked"]);
        QVector2D pos((*itr)["pos_x"], (*itr)["pos_y"]);
        joint->set_position(pos);
        l->m_joints.push_back(joint);
    }
    for (auto itr = j["links"].begin(); itr!= j["links"].end(); itr++){
        Spring2D* link = new Spring2D(l->m_parent);
        link->m_id = (*itr)["id"];
        link->set_first_joint(id_map[(*itr)["first_joint_id"]]);
        link->set_second_joint(id_map[(*itr)["second_joint_id"]]);
        link->compute(QVector2D(0,0));
        l->m_springs.push_back(link);
    }


    if(w->m_layers.size()!=0){

    }
    w->update();
}

void MainWindow::on_actionSave_triggered()
{

    // Logic for file opening
    QString filename = QFileDialog::getSaveFileName(this, tr("Save scene."), "", tr("JSON File (*.json)"));

    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
        file.errorString());
        return;
    }

    // Logic for serialization
    json j;

    A2_2DWidget* w = ui->OpenGL;
    A3_2DLayer* l = dynamic_cast<A3_2DLayer*>(w->m_layers[0]);

    j["joints"] = json::array();
    j["links"] = json::array();
    j["obstacles"] = json::array();

    for(unsigned int i =0; i<l->m_joints.size(); i++){
        Joint2D* joint = l->m_joints[i];
        json saved_joint = json::object();
        QVector2D pos = joint->get_position();
        saved_joint["pos_x"] = pos.x();
        saved_joint["pos_y"] = pos.y();
        saved_joint["id"] = joint->m_id;
        saved_joint["locked"] = joint->m_bIsLocked;
        j["joints"].push_back(saved_joint);
    }
    for(unsigned int i =0; i<l->m_springs.size(); i++){
        Spring2D* link = l->m_springs[i];
        json saved_link = json::object();
        saved_link["id"] = link->m_id;
        saved_link["first_joint_id"] = link->get_first_joint()->m_id;
        saved_link["second_joint_id"] = link->get_second_joint()->m_id;
        j["links"].push_back(saved_link);
    }

    // Completes the saving process
    QString out_string = QString::fromUtf8(j.dump(4).c_str());

    QTextStream out(&file);
    out << out_string;

}

void MainWindow::on_actionReset_View_triggered()
{
    A2_2DWidget* w = ui->OpenGL;
    if(w->m_layers.size()!=0){
        w->m_layers[0]->reset_view();
    }

     w->update();
}

void MainWindow::on_actionSelect_and_Edit_Joints_triggered()
{
    A2_2DWidget* w = ui->OpenGL;
    if(w->m_layers.size()!=0){
        w->m_layers[0]->setUIMode(1);
    }

    w->update();

}

void MainWindow::on_actionSelect_and_Edit_Links_triggered()
{
    A2_2DWidget* w = ui->OpenGL;
    if(w->m_layers.size()!=0){
        w->m_layers[0]->setUIMode(2);
    }

    w->update();
}

void MainWindow::on_actionSelect_and_Edit_Obstacles_triggered()
{
    A2_2DWidget* w = ui->OpenGL;
    if(w->m_layers.size()!=0){
        w->m_layers[0]->setUIMode(3);
    }

    w->update();
}

void MainWindow::on_actionShowtime_triggered()
{
    A2_2DWidget* w = ui->OpenGL;
    if(w->m_layers.size()!=0){
        w->m_layers[0]->setUIMode(4);
    }

    w->update();

}

void MainWindow::on_actionTest_Eigen_triggered()
{
    A3Solution::test_eigen_library();
}

void MainWindow::on_actionParameters_triggered()
{
    params->show();
    qDebug() << "HAPPENS!!";
    //params->setModal(true);
    //params->exec();
}

void MainWindow::params_updated()
{
    A2_2DWidget* w = ui->OpenGL;
    //int ms = int(params->m_timestep * 1000);
    int ms = 10;
    w->timer->stop();
    w->timer->start(ms);
    if(w->m_layers.size()!=0){
        A3_2DLayer* l = dynamic_cast<A3_2DLayer*>(w->m_layers[0]);
        l->m_gravity = params->m_gravity;
        l->m_positional_damping = params->m_positional_damping;
        l->m_mass = params->m_mass;
        l->m_timestep = params->m_timestep;
        l->m_implicit = params->m_implicit;
    }

}
