#include "ogllayer2d.h"

#include "../elements/circle.h"
#include "../widgets/oglTwidget.h"

#include <iostream>

using namespace std;

OGLLayer2D::OGLLayer2D(OGLTWidget* parent)
    :OGLLayer(parent), m_selected(0)
{

}

bool OGLLayer2D::reset_view(){
    m_view.setToIdentity();
    return true;
}

void OGLLayer2D::key_press(KeyboardInfo ki){
    (void)ki;
}

void OGLLayer2D::scroll(double delta){

    double scale = 1.0 + delta * 0.01;

    m_view.translate(QVector3D(-m_mouse_pos[0], -m_mouse_pos[1], 0));
    m_view.scale(QVector3D(scale, scale, scale));
    m_view.translate(QVector3D(m_mouse_pos[0], m_mouse_pos[1], 0));

}


void OGLLayer2D::pick(QVector2D pos, PickPrimitiveDataback& ppd){
    QRect rect;
    rect.setLeft(0);
    rect.setTop(0);
    rect.setRight(m_parent->width());
    rect.setBottom(m_parent->height());

    cout<<"Rect: "<<rect<<endl;
    ppd = GeometryUtils::prepare_pick_databack_ortho(pos[0], pos[1], -1, m_view, m_projection, rect);

    for(unsigned int i=0;i<m_elements.size();++i){
        m_elements[i]->pick(ppd);
    }


    ppd.sort();


    cout<<"I picked_: "<<ppd.m_primitives.size()<<endl;
    cout<<ppd.m_dir<<endl;

    if(ppd.m_primitives.size()>0){
        PickPrimitive pp = ppd.m_primitives[0];
        cout<<"Screen space: "<<pp.m_P3d<<endl;
        cout<<"Imag space: "<<pp.m_texture_pos<<endl;
    }

}


void OGLLayer2D::mouse_grab(MouseInfo m){

     m_mouse_pos = m.pos;

    // old stuff from splines - when I go back I need to revisit
 #if 0
    if(m_UIMode==1){

        m_selected = 0;
        for(int i=0;i<m_elements.size();++i){
            if(m_elements[i]->m_is_movable &&
                    GeometryUtils::is_inside(m.pos.x(), m.pos.y(),m_elements[i]->getBB())){
                m_selected = m_elements[i];
                break;
            }
        }

        if(!m_selected){
            Circle2D* c = new Circle2D(m_parent);
            c->compute(m.pos, 10);
            m_selected = c;
            m_elements.push_back(m_selected);
        }
    }
#endif


    if(m_UIMode==0){

    } else if(m_UIMode==1){

    } else if(m_UIMode==2){

        if(m_bPicking){
            PickPrimitiveDataback ppd;
            pick(m.pos, ppd);
            m_parent->PickPrimitiveGrab(ppd);
        }

    } else {

    }



}

void OGLLayer2D::mouse_hover(MouseInfo m){



 m_mouse_pos = m.pos;


}

void OGLLayer2D::mouse_drag(MouseInfo m){

    m_mouse_pos = m.pos;


    if(m_UIMode==0){       
        QVector3D d(m.pos[0]-m.pos0[0], m.pos[1]-m.pos0[1],0);
        m_view.translate(d);
    } else if(m_UIMode==1){
        if(m_selected){
            m_selected->mouse_drag(m);
        }
    } else if(m_UIMode==2){

        if(m_bPicking){
            PickPrimitiveDataback ppd;
            pick(m.pos, ppd);
            m_parent->PickPrimitiveDrag(ppd);
        }

    }
}



void OGLLayer2D::mouse_release(MouseInfo m){
    m_selected = 0;


    if(m_UIMode==2){
        if(m_bPicking){
            PickPrimitiveDataback ppd;
            pick(m.pos, ppd);
            m_parent->PickPrimitiveRelease(ppd);
        }

    }
}

void OGLLayer2D::mouse_double_click(MouseInfo m){
    (void)m;
}

void OGLLayer2D::resizeGL(int w, int h){

    m_projection.setToIdentity();
    m_projection.ortho(0, w, h, 0, -1, 1);

    for(unsigned int i=0;i<m_elements.size();++i){
        m_elements[i]->resize(w, h);
    }

}

bool OGLLayer2D::initializeGL(){
    reset_view();
    return true;
}


bool OGLLayer2D::draw(){

    if(!m_bIsInitGL){
        initializeGL();
        m_bIsInitGL = true;
    }


    // render all elememnts
    for(unsigned int i=0;i<m_elements.size();++i)
        m_elements[i]->draw(m_view, m_projection);


    return true;
}
