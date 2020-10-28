#include <QDebug>

#include "parameters.h"
#include "ui_parameters.h"

#include "OpenGL/widgets/A2_2Dwidget.h"
#include "OpenGL/layers/A3_2dlayer.h"

Parameters::Parameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameters)
{
    ui->setupUi(this);

    // Sets up parameters
    m_gravity = ui->gravityValue->value();
    m_positional_damping = ui->positionalDamping->value();
    m_mass = ui->mass->value();
    m_implicit = ui->implictEulerValue->isChecked();
    m_timestep = ui->timestepValue->value();
    m_gravity_new = m_gravity;
    m_positional_damping_new = m_positional_damping;
    m_mass_new = m_mass;
}

Parameters::~Parameters()
{
    delete ui;
}

void Parameters::on_buttonBox_accepted()
{
    m_gravity = m_gravity_new;
    m_positional_damping = m_positional_damping_new;
    m_mass = m_mass_new;
    m_implicit = m_implicit_new;
    m_timestep = m_timestep_new;

    emit this->dialog_accepted();


}

void Parameters::on_buttonBox_rejected()
{
    m_gravity_new = m_gravity;
    m_positional_damping_new = m_positional_damping;
    m_mass_new = m_mass;
    m_implicit_new = m_implicit;
    m_timestep_new = m_timestep;

    ui->gravityValue->setValue(m_gravity);
    ui->positionalDamping->setValue(m_positional_damping);
    ui->mass->setValue(m_mass);
    ui->implictEulerValue->setTristate(m_implicit);
    ui->timestepValue->setValue(m_timestep);
}

void Parameters::on_positionalDamping_valueChanged(double arg1)
{
    m_positional_damping_new = arg1;
}


void Parameters::on_mass_valueChanged(double arg1)
{
    m_mass_new = arg1;
}

void Parameters::on_gravityValue_valueChanged(double arg1)
{
    m_gravity_new = arg1;
}

void Parameters::showEvent(QShowEvent * event){
    QWidget::showEvent(event);
    m_gravity_new = m_gravity;
    m_positional_damping_new = m_positional_damping;
    m_mass_new = m_mass;
    m_implicit_new = m_implicit;
    m_timestep_new = m_timestep;
}



void Parameters::on_timestepValue_valueChanged(double arg1)
{
    m_timestep_new = arg1;
}

void Parameters::on_implictEulerValue_clicked(bool checked)
{
    m_implicit_new = checked;
}

