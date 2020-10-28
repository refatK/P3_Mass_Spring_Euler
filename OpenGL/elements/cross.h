#ifndef CROSS_H
#define CROSS_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "polygon.h"

#include "../geometry/geometryutils.h"

class OGLTWidget;
class Cross2D : public Polygon2D
{
public:
    Cross2D(OGLTWidget* parent);


     // Virtual functions
    virtual void mouse_drag(MouseInfo m);

    void compute(QVector2D center, float radius, int n=100);

    virtual ~Cross2D(){}

    QVector2D m_center;
    float m_radius;

};

#endif // CROSS_H
