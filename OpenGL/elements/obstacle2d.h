#ifndef OBSTACLE2D_H
#define OBSTACLE2D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "../elements/circle.h"

#include "../geometry/geometryutils.h"

class OGLTWidget;


#include "polygon.h"

class Obstacle2D : public Circle2D
{
public:
   Obstacle2D(OGLTWidget* parent);
   virtual ~Obstacle2D(){}

    // Virtual functions
   virtual void mouse_drag(MouseInfo m);

   void update();

};

#endif // OBSTACLE2D_H
