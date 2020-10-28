#include "spring2d.h"
#include "joint2D.h"

#include <cmath>
#include <QTransform>
#include <QDebug>
#include <QElapsedTimer>


Spring2D::Spring2D(OGLTWidget* parent)
    :Polygon2D(parent)
{

    m_joints[0] = nullptr;
    m_joints[1] = nullptr;
    m_thickness = 4;
    m_length = 0;
    m_rest_length = -1;
}


Spring2D::~Spring2D(){

}

void Spring2D::mouse_drag(MouseInfo m){
    compute(m.pos);
}

void Spring2D::mouse_grab(MouseInfo m){
    (void)m;
}

void Spring2D::mouse_release(MouseInfo m){
    (void)m;
}

bool Spring2D::is_inside(QVector2D pos)const{


    if (!GeometryUtils::is_inside(pos.x(), pos.y(),getBB())){
        return false;
    }

    float a = m_joints[0]->m_pos.y()-m_joints[1]->m_pos.y();
    float b = m_joints[1]->m_pos.x()-m_joints[0]->m_pos.x();
    float c = (m_joints[0]->m_pos.x()-m_joints[1]->m_pos.x())*m_joints[0]->m_pos.y() + (m_joints[1]->m_pos.y()-m_joints[0]->m_pos.y())*m_joints[0]->m_pos.x();

    float distance = abs(a*pos.x()+ b*pos.y() + c)/sqrt(a*a + b*b);

    if (distance < (float)m_thickness){
        return true;
    } else {
        return false;
    }

}

void Spring2D::set_first_joint(Joint2D* joint)
{
    m_joints[0] = joint;
    m_joints[0]->add_spring(this);
}

void Spring2D::set_second_joint(Joint2D* joint)
{
    m_joints[1] = joint;
    m_joints[1]->add_spring(this);
    m_rest_length = (m_joints[1]->get_position() - m_joints[0]->get_position()).length();
}

Joint2D* Spring2D::get_first_joint(){
    return m_joints[0];
}

Joint2D* Spring2D::get_second_joint(){
    return m_joints[1];
}

Joint2D* Spring2D::get_other_joint(Joint2D* joint){

    if (joint == m_joints[0]){
        return m_joints[1];
    } else if (joint == m_joints[1]){
        return m_joints[0];
    } else {
        return nullptr;
    }
}


float Spring2D::get_length()
{
    return m_length;
}

float Spring2D::get_rest_length()
{
    return m_rest_length;
}

void Spring2D::compute(QVector2D pos){

    //QElapsedTimer t;
    //t.start();
    if (m_joints[0] == nullptr)
        return;

    m_curve.clear();

    QVector2D pos1;
    QVector2D pos2;

    pos1 = m_joints[0]->get_position();
    if (m_joints[1] == nullptr)
    {
        pos2 = pos;
    } else {
        pos2 = m_joints[1]->get_position();
    }

    QVector2D diff = pos2 - pos1;
    QVector2D unit = diff.normalized();

    // Sets parameters
    m_length = diff.length();

    // Number of zig zag sections
    int zigzag_length = 20;
    float zigzag_size = 4.0f;
    int n = 100;
    int sep = 5;

    if (this->get_second_joint() == nullptr){
        zigzag_length = 20;
        zigzag_size = 4.0f;
        n = static_cast<int>(diff.length());
    } else {
        zigzag_length = 20;
        zigzag_size = 4.0f;
        n = static_cast<int>(m_rest_length);
    }

    for(int i=1;i<=n;++i){
        QVector2D offset = QVector2D(unit.y(), -unit.x());

        if (i % zigzag_length < sep){
            offset = ((float)(i % sep))/((float)sep) * offset;
        } else if (i % zigzag_length < 2*sep){
            offset = ((float)(sep - (i % sep)))/((float)sep) * offset;
        } else if (i % zigzag_length < 3*sep){
            offset = -((float)(i % sep))/((float)sep) * offset;
        } else if (i % zigzag_length < 4*sep){
            offset = -((float)(sep - (i % sep)))/((float)sep) * offset;
        }
        offset = zigzag_size * offset;

        QVector2D newPos = pos1 + ((float)i / (float)n)*diff + offset;
        VertexData v;

        v.position = newPos;

        m_curve.push_back(v);
    }

    //qDebug() << "Compute takes: " << t.nsecsElapsed() << " ns.";
}

