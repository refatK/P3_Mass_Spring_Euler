#include "A2_2Dwidget.h"

#include <QMouseEvent>

#include <math.h>

#include <iostream>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>


#include "../layers/A3_2dlayer.h"

using namespace std;



A2_2DWidget::A2_2DWidget(QWidget *parent) :
    OGLTWidget(parent)
{

   A3_2DLayer* layer = new A3_2DLayer(this);
   m_layers.push_back(layer);

   timer = new QTimer(this);
   QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update_physics()));

}

A2_2DWidget::~A2_2DWidget()
{

}

void A2_2DWidget::keyPressEvent(QKeyEvent *event){


    OGLTWidget::keyPressEvent(event);
}


void A2_2DWidget::keyReleaseEvent(QKeyEvent *event){


    OGLTWidget::keyReleaseEvent(event);
}


void A2_2DWidget::update_physics(){

    //timer->
    A3_2DLayer* l = dynamic_cast<A3_2DLayer*>(m_layers[0]);
    //l->m_timestep = elapsed;
    //qDebug() << "Called!";
    l->update_physics();
    update();
}
