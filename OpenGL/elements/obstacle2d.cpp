#include "obstacle2d.h"


#include <iostream>

#include <cmath>

using namespace std;


Obstacle2D::Obstacle2D(OGLTWidget* parent)
    :Circle2D(parent)
{

}

void Obstacle2D::mouse_drag(MouseInfo m){
    Circle2D::mouse_drag(m);
}
