#ifndef DRAWELEMENT2D_H
#define DRAWELEMENT2D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "drawelement.h"

#include "../geometry/geometryutils.h"

class OGLTWidget;
class DrawElement2D : public DrawElement
{
public:
    DrawElement2D(OGLTWidget* parent);

    // common elements

    // can it be moved by the GUI


    QVector3D m_color;// rgb in the range [0, 1]
    virtual QRect getBB()const=0;
    virtual bool is_inside(QVector2D pos)const=0;


    virtual void mouse_grab(MouseInfo m)=0;
    virtual void mouse_drag(MouseInfo m)=0;
    virtual void mouse_release(MouseInfo m)=0;


    // virtual
    virtual bool initializeGL()=0;
    virtual bool draw(QMatrix4x4 model, QMatrix4x4 projection)=0;
    virtual bool updateOGLBuffer()=0;
    virtual ~DrawElement2D(){}


    // load / save later as it is trickier
    // not virtual because I can add a control mechanism to see if I can load/sav crrectly
    // control words
    virtual bool save(QDataStream* out)=0;
    virtual bool load(QDataStream* out)=0;
};

#endif // DRAWELEMENT2D_H
