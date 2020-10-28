#ifndef OGLLAYER2D_H
#define OGLLAYER2D_H

#include "ogllayer.h"

#include "../3D/pickprimitivedataback.h"

#include "../elements/drawelement2D.h"

class OGLLayer2D : public OGLLayer
{
public:
    OGLLayer2D(OGLTWidget* parent);


        // UI
    virtual void mouse_grab(MouseInfo m);
    virtual void mouse_drag(MouseInfo m);
    virtual void mouse_release(MouseInfo m);
    virtual void mouse_hover(MouseInfo m);
    virtual void mouse_double_click(MouseInfo m);

    virtual void key_press(KeyboardInfo ki);
    virtual void key_release(KeyboardInfo ki){(void)ki;}

    virtual void scroll(double delta);


    virtual bool reset_view();

    virtual void resizeGL(int w, int h);

       // virtual
    virtual bool initializeGL();
    virtual bool draw();

    virtual bool passMsg(TMessage*){return false;};

    virtual ~OGLLayer2D(){}

    // load / save later as it is trickier
    // not virtual because I can add a control mechanism to see if I can load/sav crrectly
    // control wor
    virtual bool save(QDataStream* out){(void)out;return false;};
    virtual bool load(QDataStream* out){(void)out;return false;};


    void pick(QVector2D pos, PickPrimitiveDataback& ppd);

    QMatrix4x4 m_projection, m_view;

    QVector2D m_mouse_pos;

    std::vector<DrawElement2D*> m_elements;
    DrawElement2D* m_selected;

    };


#endif // OGLLAYER2D_H
