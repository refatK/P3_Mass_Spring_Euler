#ifndef POLYGONE2D_H
#define POLYGONE2D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "drawelement2D.h"

#include "../geometry/geometryutils.h"

class OGLTWidget;
class Polygon2D : public DrawElement2D
{
public:
    Polygon2D(OGLTWidget* parent);


    // NON Opengl stuff
    std::vector<VertexData> m_curve;


    // OpenGL stuff
    QOpenGLBuffer m_lineBuf;
    QOpenGLBuffer m_indexlineBuf;
    QOpenGLVertexArrayObject m_vao;

    // Virtual functions
    virtual QRect getBB()const;
    virtual bool is_inside(QVector2D pos)const;

    virtual void mouse_grab(MouseInfo m);
    virtual void mouse_drag(MouseInfo m);
    virtual void mouse_release(MouseInfo m);


    // virtual
    virtual bool initializeGL();
    virtual bool draw(QMatrix4x4 model, QMatrix4x4 projection);
    virtual bool updateOGLBuffer();
    virtual ~Polygon2D(){}


    // load / save later as it is trickier
    // not virtual because I can add a control mechanism to see if I can load/sav crrectly
    // control words
    virtual bool save(QDataStream* out);
    virtual bool load(QDataStream* out);

protected:
    float m_thickness;

};

#endif // DRAWELEMENT2D_H
