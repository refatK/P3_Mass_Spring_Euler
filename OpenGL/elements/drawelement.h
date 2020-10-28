#ifndef DRAWELEMENT_H
#define DRAWELEMENT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "../geometry/geometryutils.h"
#include "../../MessagePassing/messagemanager.h"

#include "../3D/pickprimitivedataback.h"

class OGLTWidget;
class DrawElement : protected QOpenGLFunctions
{
public:
    DrawElement(OGLTWidget* parent);

    // common elements

    // can it be moved by the GUI
    bool m_is_movable;
    bool m_bHidden;
    bool m_bUI;// if true the UI is passed
    bool m_bPicking;// if true it means we can pick objects

    std::string m_id;

    bool m_bUpdateBuffer;
    bool m_bIsInitGL;


    OGLTWidget* m_parent = nullptr;

    virtual void mouse_grab(MouseInfo m)=0;
    virtual void mouse_drag(MouseInfo m)=0;
    virtual void mouse_release(MouseInfo m)=0;


    virtual void resize(int w, int h){(void)w;(void)h;}

    // virtual
    virtual bool initializeGL()=0;
    virtual bool draw(QMatrix4x4 model, QMatrix4x4 projection)=0;
    virtual bool updateOGLBuffer()=0;
    virtual ~DrawElement(){}


    // picking business
    // I dont make it virtual but returns -1 if not implemented, otherwise the number of intersections
    virtual int pick(PickPrimitiveDataback& ppd) {(void) ppd; return -1;}

    virtual bool passMsg(TMessage*){return false;}

    // load / save later as it is trickier
    // not virtual because I can add a control mechanism to see if I can load/sav crrectly
    // control words
    virtual bool save(QDataStream* out)=0;
    virtual bool load(QDataStream* out)=0;

    static bool saveVertexData(QDataStream* out, const VertexData& data);
    static bool loadVertexData(QDataStream* in, VertexData& data);
};

#endif // DRAWELEMENT2D_H
