#ifndef OGLLAYER_H
#define OGLLAYER_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>


#include "../geometry/geometryutils.h"

#include "../../MessagePassing/messagemanager.h"


class OGLTWidget;
class OGLLayer : protected QOpenGLFunctions
{
public:

    OGLLayer(OGLTWidget* parent);

    bool m_bIsInitGL;
    OGLTWidget* m_parent;

    // if true then it is hidden
    bool m_bHidden;
    bool m_bUI;// if true the UI is passed
    bool m_bPicking;// if true it means we can pick objects

    // UI
    virtual void mouse_grab(MouseInfo m)=0;
    virtual void mouse_drag(MouseInfo m)=0;
    virtual void mouse_release(MouseInfo m)=0;
    virtual void mouse_hover(MouseInfo m)=0;
    virtual void mouse_double_click(MouseInfo m)=0;

    virtual void key_press(KeyboardInfo ki)=0;
    virtual void key_release(KeyboardInfo ki)=0;

    virtual void scroll(double delta)=0;


    // virtual
    virtual void resizeGL(int w, int h)=0;
    virtual bool initializeGL()=0;
    virtual bool draw()=0;
    virtual bool reset_view()=0;

    virtual ~OGLLayer(){}

    // load / save later as it is trickier
    // not virtual because I can add a control mechanism to see if I can load/sav crrectly
    // control words
    virtual bool save(QDataStream* out)=0;
    virtual bool load(QDataStream* out)=0;

    // 0 = change the view, 1 = create and select,  2 = pick a point
    int m_UIMode;
    virtual bool setUIMode(int m){
       m_UIMode = m;
       return true;
    }


    virtual bool passMsg(TMessage*)=0;




};



#endif // OGLLAYER_H
