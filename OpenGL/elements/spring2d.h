#ifndef LINK2D_H
#define LINK2D_H

#include "polygon.h"

class Joint2D;

class Spring2D : public Polygon2D
{
public:
    Spring2D(OGLTWidget* parent);

    virtual void mouse_grab(MouseInfo m);
    virtual void mouse_drag(MouseInfo m);
    virtual void mouse_release(MouseInfo m);
    virtual bool is_inside(QVector2D pos)const;

    void compute(QVector2D pos);

    void set_first_joint(Joint2D* joint);
    void set_second_joint(Joint2D* joint);
    Joint2D* get_first_joint();
    Joint2D* get_second_joint();

    Joint2D* get_other_joint(Joint2D* joint);

    float get_length();
    float get_rest_length();

    virtual ~Spring2D();

private:
    Joint2D* m_joints[2];

    float m_length;
    float m_rest_length;


};

#endif // LINK2D_H
