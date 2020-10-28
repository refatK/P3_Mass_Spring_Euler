#include "oglTwidget.h"

#include <QMouseEvent>
#include <QMainWindow>

#include <math.h>

#include <iostream>

#include "../layers/ogllayer2d.h"



using namespace std;

extern int global_scale;

OGLTWidget::OGLTWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_button_pressed(false)
{
     setFocusPolicy(Qt::ClickFocus);
    setMouseTracking(true);
}


std::string OGLTWidget::getWindowTitle2(){
    std::string ret="INIT";

    QWidget* t = this;
    do{
        if(!t){
            ret = "ERROR";
            break;
        }

        QMainWindow* q = dynamic_cast<QMainWindow*> (t);
        if(q){
        //if(t->windowType()==Qt::Window || t->windowType()==Qt::Dialog){
            ret = std::string(q->windowTitle().toUtf8().constData());
            break;
        }

        t = t->parentWidget();
        cout<<"cur"<<endl;
    }while(1);


    return ret;

}


OGLTWidget::~OGLTWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    doneCurrent();
}

void OGLTWidget::keyPressEvent(QKeyEvent *event){

    KeyboardInfo ki;

    bool m_bpassdown = true;

    QString qs = event->text();
    std::string s = qs.toUtf8().constData();
    ki.m_k = s[0];

    switch(ki.m_k){

    case 'c':
        save_frame_buffer("/Users/stpopa/Documents/ProjectData/AudioZ/Result/test.png");
        m_bpassdown = false;
    break;
    case 'v':
        if(m_layers.size()!=0){
            m_layers[0]->setUIMode(0);
        }
         m_bpassdown = false;
        break;
    case 's':
        if(m_layers.size()!=0){
            m_layers[0]->setUIMode(1);
        }
         m_bpassdown = false;
    case 'p':
        if(m_layers.size()!=0){
            m_layers[0]->setUIMode(2);
        }
         m_bpassdown = false;
        break;
    }

    if(m_bpassdown){
        for(unsigned int i=0;i<m_layers.size();++i){
            if(!m_layers[i]->m_bHidden)
                m_layers[i]->key_press(ki);
        }
    }

    update();
}


void OGLTWidget::keyReleaseEvent(QKeyEvent *event){

     bool m_bpassdown = true;

    KeyboardInfo ki;

    QString qs = event->text();
    std::string s = qs.toUtf8().constData();
    ki.m_k = s[0];

    if(m_bpassdown){
        for(unsigned int i=0;i<m_layers.size();++i){
            if(!m_layers[i]->m_bHidden)
                m_layers[i]->key_release(ki);
        }
    }

    update();

}

void OGLTWidget::wheelEvent(QWheelEvent *event){
    QPoint p = event->pixelDelta();

    for(unsigned int i=0;i<m_layers.size();++i){
        m_layers[i]->scroll(p.y());
    }

    update();
}


void OGLTWidget::mousePressEvent(QMouseEvent *e)
{

    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    prevMouseMovePosition = mousePressPosition;
    m_button_pressed = true;

    m_mouse.pos0 = m_mouse.pos =   mousePressPosition;
    if(e->button()==Qt::LeftButton){
        m_mouse.button = 0;
    } else if(e->button()==Qt::MiddleButton){
        m_mouse.button = 1;
    } else if(e->button()==Qt::RightButton){
        m_mouse.button = 2;
    } else {
        cout<<"Unknown button!"<<endl;
    }

    for(unsigned int i=0;i<m_layers.size();++i){
        m_layers[i]->mouse_grab(m_mouse);
    }

    update();
}

void OGLTWidget::mouseReleaseEvent(QMouseEvent *e)
{

    m_mouse.pos0 = prevMouseMovePosition;
    m_mouse.pos =   mousePressPosition;
    if(e->button()==Qt::LeftButton){
        m_mouse.button = 0;
    } else if(e->button()==Qt::MiddleButton){
        m_mouse.button = 1;
    } else if(e->button()==Qt::RightButton){
        m_mouse.button = 2;
    } else {
    }

    for(unsigned int i=0;i<m_layers.size();++i){
        if(!m_layers[i]->m_bHidden)
            m_layers[i]->mouse_release(m_mouse);
    }

    m_button_pressed = false;
    update();
}


void OGLTWidget::mouseMoveEvent(QMouseEvent *event){

    mousePressPosition = QVector2D(event->localPos());

    m_mouse.pos0 = prevMouseMovePosition;
     m_mouse.pos =   mousePressPosition;
    if(event->button()==Qt::LeftButton){
        m_mouse.button = 0;
    } else if(event->button()==Qt::MiddleButton){
        m_mouse.button = 1;
    } else if(event->button()==Qt::RightButton){
        m_mouse.button = 2;
    } else {
      // TODO: the mouse button sometiemes comes incorrect from e
        //cout<<"Unknown button!"<<endl;
    }

    if(m_button_pressed){
        for(unsigned int i=0;i<m_layers.size();++i){
            if(!m_layers[i]->m_bHidden)
                m_layers[i]->mouse_drag(m_mouse);
        }

    } else {
        for(unsigned int i=0;i<m_layers.size();++i){
            if(!m_layers[i]->m_bHidden)
                m_layers[i]->mouse_hover(m_mouse);
        }
    }


    prevMouseMovePosition = mousePressPosition;
    update();
}

void OGLTWidget::mouseDoubleClickEvent(QMouseEvent *event){

    // Save mouse double clickpress position
    mousePressPosition = QVector2D(event->localPos());
    prevMouseMovePosition = mousePressPosition;
    m_button_pressed = true;

    m_mouse.pos0 = m_mouse.pos =   mousePressPosition;
    if(event->button()==Qt::LeftButton){
        m_mouse.button = 0;
    } else if(event->button()==Qt::MiddleButton){
        m_mouse.button = 1;
    } else if(event->button()==Qt::RightButton){
        m_mouse.button = 2;
    } else {
        cout<<"Unknown button!"<<endl;
    }

    for(unsigned int i=0;i<m_layers.size();++i){
        m_layers[i]->mouse_double_click(m_mouse);
    }

    update();

}


void OGLTWidget::timerEvent(QTimerEvent *)
{
}


bool OGLTWidget::save_frame_buffer(std::string f){
    QImage image;
    image = grabFramebuffer();

    QString qs =  QString::fromUtf8(f.c_str());

    image.save(qs);

    return true;
}


// load / save

void OGLTWidget::readCurves(QDataStream* in){
    (void)in;
    update();
}

void OGLTWidget::writeCurves(QDataStream* out){
    (void)out;
}


void OGLTWidget::SendMessages(TMessage*){
    for(unsigned int i=0;i<m_layers.size();++i){
        if(!m_layers[i]->m_bHidden){
        }
    }
}


