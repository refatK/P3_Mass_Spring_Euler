#include "circle.h"

#include "../widgets/oglTwidget.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using namespace std;

Circle2D::Circle2D(OGLTWidget* parent)
    :Polygon2D(parent), m_radius(0)
{
    m_thickness = 2;

}


void Circle2D::mouse_drag(MouseInfo m){
    Polygon2D::mouse_drag(m);

    m_center[0]+=m.pos[0]-m.pos0[0];
    m_center[1]+=m.pos[1]-m.pos0[1];
}


void Circle2D::compute(QVector2D center, float radius, int n){

    m_center = center;
    m_radius = radius;
    m_curve.clear();


    float delta = 2.0f*M_PI / (float)(n);

    for(int i=0;i<=n;++i){
        VertexData v;
        QVector2D q(cosf((float)i * delta), sinf((float)i * delta));
        v.position = center + radius * q;
        m_curve.push_back(v);
    }

}
