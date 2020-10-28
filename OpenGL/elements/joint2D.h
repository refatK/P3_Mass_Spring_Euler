#ifndef BLOSSOM_POINT_H
#define BLOSSOM_POINT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "../elements/circle.h"
#include "../elements/cross.h"

#include "../geometry/geometryutils.h"

#include <vector>
#include "dependencies/Eigen/Dense"
using Eigen::Vector2f;

class OGLTWidget;

class Spring2D;

class Joint2D: public Polygon2D
{
public:
    Joint2D(OGLTWidget* parent);

    friend class Spring2D;

     // Virtual functions
    virtual void mouse_drag(MouseInfo m);
    virtual bool draw(QMatrix4x4 model, QMatrix4x4 projection);
    virtual QRect getBB()const;

    std::vector<Spring2D*> get_springs() {return m_springs;}
    void add_spring(Spring2D* link);
    void remove_spring(Spring2D* link);

    bool is_locked() {return m_bIsLocked;}
    void set_locked(bool value);
    void set_position(QVector2D pos);
    void set_position(Vector2f pos);

    QVector2D get_position(){return m_pos;}

    bool m_bIsLocked;

    virtual ~Joint2D();

private:

    // Visual elements
    Cross2D* m_cross;
    Circle2D* m_circle;

    // Connection elements
    QVector2D m_pos;
    std::vector<Spring2D*> m_springs;


};

#endif //
