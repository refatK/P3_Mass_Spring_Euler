#ifndef CIRCLE2D_H
#define CIRCLE2D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "polygon.h"

#include "../geometry/geometryutils.h"

class OGLTWidget;
class Circle2D : public Polygon2D
{
public:
    Circle2D(OGLTWidget* parent);

     // Virtual functions
    virtual void mouse_drag(MouseInfo m);

    void compute(QVector2D center, float radius, int n = 100);

    virtual ~Circle2D(){}

    QVector2D m_center;
    float m_radius;

};

#endif // DRAWELEMENT2D_H
