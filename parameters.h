#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QDialog>

namespace Ui {
class Parameters;
}

class Parameters : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters(QWidget *parent = nullptr);
    ~Parameters();

signals:
    void dialog_accepted();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_positionalDamping_valueChanged(double arg1);

    void on_gravityValue_valueChanged(double arg1);

    void showEvent(QShowEvent *);

    void on_timestepValue_valueChanged(double arg1);

    void on_implictEulerValue_clicked(bool checked);

    void on_mass_valueChanged(double arg1);

    void on_stiffness_valueChanged(double arg1);

public:
    // Actual stored parameters
    float m_gravity;
    float m_positional_damping;
    float m_mass;
    float m_timestep;
    float m_stiffness;
    bool m_implicit;

private:
    Ui::Parameters *ui;

    // Temp parameters, only applied when (OK) button is clicked
    float m_gravity_new;
    float m_positional_damping_new;
    float m_mass_new;
    float m_timestep_new;
    float m_stiffness_new;
    bool m_implicit_new;
};

#endif // PARAMETERS_H
