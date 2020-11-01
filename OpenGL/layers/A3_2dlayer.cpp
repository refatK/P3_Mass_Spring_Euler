#include "A3_2dlayer.h"

#include "../elements/circle.h"
#include "../elements/joint2D.h"

#include <iostream>

#include <QTimer>
#include <QObject>

using namespace std;



A3_2DLayer::A3_2DLayer(OGLTWidget* parent)
    :OGLLayer(parent), m_UIMode(1), m_selected(0),
      m_solution(m_joints, m_springs, m_gravity, m_positional_damping, m_mass, m_timestep, m_implicit, m_stiffness)
{
}

void A3_2DLayer::mouse_hover(MouseInfo m){
    (void)m;
}

bool A3_2DLayer::setUIMode(int m){
    (void)m;
    m_UIMode = m;
    return true;
}

bool A3_2DLayer::reset_view(){
    m_view.setToIdentity();
    return true;
}


void A3_2DLayer::key_press(KeyboardInfo ki){
    (void)ki;
}


void A3_2DLayer::key_release(KeyboardInfo ki){
    (void)ki;
}



void A3_2DLayer::scroll(double delta){
    (void)delta;
}



void A3_2DLayer::mouse_grab(MouseInfo m){


    if(m_UIMode==1){

        // Joint place / move mode
        m_selected = nullptr;
        Joint2D* selected_joint = nullptr;
        for(unsigned int i=0;i<m_joints.size();++i){
            if(m_joints[i]->m_is_movable && m_joints[i]->is_inside(m.pos)){
                selected_joint = m_joints[i];
                break;
            }
        }

        if (m.button == 0){
            if (!selected_joint){
                Joint2D* new_joint = new Joint2D(m_parent);
                new_joint->set_position(m.pos);
                m_joints.push_back(new_joint);
                m_selected = new_joint;
            } else {
                m_selected = selected_joint;
            }
        } else  if (m.button == 2 && selected_joint){
            for(auto it2 = selected_joint->get_springs().begin(); it2 != selected_joint->get_springs().end(); ++it2){
                for(auto it = m_springs.begin(); it != m_springs.end(); ++it){
                    if (*it == *it2){
                        m_springs.erase(it);
                        break;
                    }
                }
            }
            for(auto it = m_joints.begin(); it != m_joints.end(); ++it){
                if (*it == selected_joint){
                    m_joints.erase(it);
                    break;
                }
            }
            delete selected_joint;
        }
    } else if(m_UIMode==2){

        if (m.button == 0){

            // Start/connect joints
            m_selected = nullptr;

            Joint2D* start_joint = nullptr;

            for(unsigned int i=0;i<m_joints.size();++i){
                if(m_joints[i]->m_is_movable && m_joints[i]->is_inside(m.pos)){
                    start_joint = m_joints[i];
                    break;
                }
            }

            if (start_joint != nullptr){
                Spring2D* l = new Spring2D(m_parent);
                l->set_first_joint(start_joint);
                l->compute(m.pos);
                m_springs.push_back(l);
                m_selected = l;
            }


        } else if (m.button == 2){

            Spring2D* link = nullptr;
            for(unsigned int i=0;i<m_springs.size();++i){
                if (m_springs[i]->m_is_movable && m_springs[i]->is_inside(m.pos)){
                    link = m_springs[i];
                    break;
                }
            }

            if (link != nullptr){
                // Remove link from joints
                link->get_first_joint()->remove_spring(link);
                link->get_second_joint()->remove_spring(link);

                for(auto it = m_springs.begin(); it != m_springs.end(); ++it){
                    if (*it == link){
                        m_springs.erase(it);
                        break;
                    }
                }
                delete link;
            }
        }

    } else  if(m_UIMode==4){

        // Joint place / move mode
        m_selected = nullptr;
        for(unsigned int i=0;i<m_joints.size();++i){

            if(!m_joints[i]->is_locked() && m_joints[i]->is_inside(m.pos)){
                m_selected = m_joints[i];
                break;
            }
        }
    } else {
        cout<<"Warning: unknown mode! "<<m_UIMode<<endl;
    }

}


void A3_2DLayer::mouse_drag(MouseInfo m){

    if(m_UIMode==0){

    } else if(m_UIMode==1){
        if(m_selected){
            m_selected->mouse_drag(m);
        }
    } else if(m_UIMode==2){
        if(m_selected){
            ((Spring2D*)m_selected)->compute(m.pos);
        }
    } else if(m_UIMode==3){
        if(m_selected){
            m_selected->mouse_drag(m);
        }
    }
    else if(m_UIMode==4){
        if(m_selected){
            m_solution.update(dynamic_cast<Joint2D*>(m_selected), m.pos);
            draw();
        }
    }

}


void A3_2DLayer::mouse_release(MouseInfo m){

    if(m_UIMode==0){


    } else if(m_UIMode==1){

        if(m_selected){

        }
    } else if(m_UIMode==2){

        if(m_selected){
            Joint2D* end_joint = nullptr;
            for(unsigned int i=0;i<m_joints.size();++i){
                if(m_joints[i]->m_is_movable && GeometryUtils::is_inside(m.pos.x(), m.pos.y(),m_joints[i]->getBB())){
                    end_joint = m_joints[i];
                    break;
                }
            }

            if (end_joint == ((Spring2D*)m_selected)->get_first_joint() || end_joint == nullptr || ((Spring2D*)m_selected)->get_first_joint() == end_joint)
            {
                // Invalid link, delete
                Spring2D* l = m_springs[m_springs.size()-1];
                l->get_first_joint()->remove_spring(l);
                m_springs.pop_back();
                delete l;
            } else {
                // Otherwise add second joint to link
                ((Spring2D*)m_selected)->set_second_joint(end_joint);
                ((Spring2D*)m_selected)->compute(m.pos);
            }

        }

    }

    m_selected = 0;
}


void A3_2DLayer::mouse_double_click(MouseInfo m){

    if(m_UIMode==1){
        // Joint locking

        m_selected = 0;
        for(unsigned int i=0;i<m_joints.size();++i){
            if(GeometryUtils::is_inside(m.pos.x(), m.pos.y(),m_joints[i]->getBB())){
                m_joints[i]->set_locked(!this->m_joints[i]->m_bIsLocked);
                break;
            }
        }

    }
}


void A3_2DLayer::resizeGL(int w, int h){

    m_projection.setToIdentity();
    m_projection.ortho(0, w, h, 0, -1, 1);

}


bool A3_2DLayer::initializeGL(){

    reset_view();


    return true;
}


bool A3_2DLayer::draw(){

    if(!m_bIsInitGL){
        initializeGL();
        m_bIsInitGL = true;
    }


    // render all elememnts
    for(unsigned int i=0;i<m_joints.size();++i)
        m_joints[i]->draw(m_view, m_projection);
    for(unsigned int i=0;i<m_springs.size();++i)
        m_springs[i]->draw(m_view, m_projection);

    return true;

}

void A3_2DLayer::update_physics()
{
    if(m_UIMode==4){
        m_solution.update();
        draw();
    }
}
