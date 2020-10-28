#ifndef PICKPRIMITIVEDATABACK_H
#define PICKPRIMITIVEDATABACK_H

#include <QVector2D>
#include <QVector3D>
#include <vector>

class PickPrimitive {
public:

    QVector3D m_P3d;// global 3D position
    QVector3D m_texture_pos;// texture position
    QVector3D m_bary;
    int m_pid;// primitive ID

    int m_primitive; // 0 - triangle, 1 - point, 3 - line

    double m_d; // depth along the ray

    // some special cases
 //   QVector2D P2d;// texture space [0, 1]x[0,1]

};


class PickPrimitiveDataback
{

public:
        PickPrimitiveDataback(){

        }


        // input

        QVector3D m_dir;// ray direction in world coordinate system
        QVector3D m_origin;// ray origin

        QVector2D m_mouse_pos;

        // output

       std::vector<PickPrimitive> m_primitives;

       void sort();
       void initialize();


};

#endif // PICKPRIMITIVEDATABACK_H
