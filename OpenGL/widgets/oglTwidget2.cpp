#include "oglTwidget.h"
#include <QTabWidget>
#include <QMouseEvent>
#include <math.h>
#include <iostream>
#include "../managers/shadermanager.h"
#include "../managers/texturemanager.h"

#include <QDebug>
#include <QElapsedTimer>
#include <stdlib.h>

using namespace std;


int global_scale = 2.0;


bool OGLTWidget::initShaders()
{

    bool bret = true;

    if(!m_sm.addProgram("line_shader", ":/OpenGL/shaders/vline.glsl", ":/OpenGL/shaders/fline.glsl")){
        bret =  false;
    }


    if(!m_sm.addProgram("texture_shader", ":/OpenGL/shaders/vshader.glsl", ":/OpenGL/shaders/fshader.glsl")){
        bret =  false;
    }

    if(!m_sm.addProgram("phong_shader", ":/OpenGL/shaders/vphong.glsl", ":/OpenGL/shaders/fphong.glsl")){
        bret =  false;
    }

    if(!m_sm.addProgram("points_shader", ":/OpenGL/shaders/vshaderpoints.glsl", ":/OpenGL/shaders/fshaderpoints.glsl")){
        bret =  false;
    }

    if(!m_sm.addProgram("points_shader", ":/OpenGL/shaders/vshaderpoints.glsl", ":/OpenGL/shaders/fshaderpoints.glsl")){
        bret =  false;
    }


    return bret;
}


bool OGLTWidget::initTextures(){
    return true;

}


void OGLTWidget::initializeGL()
{

    // m_context->makeCurrent(this);
    makeCurrent();

    // preliminaries
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);


    // Enable depth buffer
    glDisable(GL_DEPTH_TEST);

    // Enable back face culling
    glDisable(GL_CULL_FACE);



    // GL initialization
    if(!initShaders()){
    }


    if(!initTextures()){
    }


}




void OGLTWidget::resizeGL(int w, int h)
{
    for(unsigned int i=0;i<m_layers.size();++i){
        m_layers[i]->resizeGL(w, h);
    }
}

void OGLTWidget::paintGL()
{
    makeCurrent();

    QSysInfo info;
    int gs = global_scale;
    if(info.productType()=="osx"){
        glViewport(0,0,gs*width(),gs*height());

    } else {
       glViewport(0,0,width(),height());
    }


    glClearColor(1, 1, 1, 0);

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render all elememnts
    for(unsigned int i=0;i<m_layers.size();++i)
        m_layers[i]->draw();

}


void OGLTWidget::glError()
{

     GLenum err = glGetError();

     if(err!=GL_NO_ERROR){
         cout<<"error in draw elementsA"<<endl;

         switch(err){
             case GL_INVALID_ENUM:
                 cout<<"GL_INVALID_ENUM"<<endl;
                 break;
             case GL_INVALID_VALUE:
                 cout<<"GL_INVALID_VALUE"<<endl;
                 break;
             case GL_INVALID_OPERATION:
                 cout<<"GL_INVALID_OPERATION"<<endl;
                 break;
             case GL_INVALID_FRAMEBUFFER_OPERATION:
                 cout<<"GL_INVALID_FRAMEBUFFER_OPERATION"<<endl;
                 break;
             case GL_OUT_OF_MEMORY:
                 cout<<"GL_OUT_OF_MEMORY"<<endl;
                 break;
             case GL_STACK_UNDERFLOW:
                 cout<<"GL_STACK_UNDERFLOW"<<endl;
                 break;
             case GL_STACK_OVERFLOW:
                 cout<<"GL_STACK_OVERFLOW"<<endl;
                 break;
             default:
                 cout<<"Unknown error!"<<endl;
         }
     }
}

