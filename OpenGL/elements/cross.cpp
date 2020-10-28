#include "cross.h"

#include "../widgets/oglTwidget.h"

#include <iostream>

#include <cmath>

using namespace std;

Cross2D::Cross2D(OGLTWidget* parent)
    :Polygon2D(parent), m_radius(0)
{
    m_thickness = 3;
}


void Cross2D::mouse_drag(MouseInfo m){
    Polygon2D::mouse_drag(m);

    m_center[0]+=m.pos[0]-m.pos0[0];
    m_center[1]+=m.pos[1]-m.pos0[1];
}


void Cross2D::compute(QVector2D center, float radius, int n){

    m_center = center;
    m_radius = radius;
    m_curve.clear();


    int div = n / 6;

    for(int i=0;i<n;++i){
        VertexData v;
        QVector2D q(0,0);
        int rel = i % div;

        if (i < 2*div){
            q.setY((-div + (float)i)/(float)div);
        } else if (i < 3*div){
            q.setY(((float)div - (float)rel)/(float)div);
        } else if (i < 4*div){
            q.setX(-1.0f*(float)rel/(float)div);
        } else if (i < 5*div){
            q.setX((-(float)div + (float)rel)/(float)div);
        } else {
            q.setX((float)rel/(float)div);
        }
        v.position = center + q * radius;

        m_curve.push_back(v);
    }

}
