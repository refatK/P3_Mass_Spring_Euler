#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "../3D/pickprimitivedataback.h"

#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

struct VertexData3D
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};


class KeyboardInfo{
public:

    char m_k;

};

class MouseInfo{
public:
    QVector2D pos0, pos;

    int button; // as below

    // NOT YET SUPPORTED
    bool buttonstatus[3];// bbutto 0 - left, 1 - middle, 2 - right

    bool isShift;
    bool isCtrl;
    bool isCommand;

    MouseInfo(){
        button = 0;
        buttonstatus[0] = buttonstatus[1] = buttonstatus[2] = false;

        isShift = false;
        isCtrl = false;
        isCommand = false;
    }

};


class GeometryUtils
{
public:
    GeometryUtils();


    static std::vector<VertexData>  createSimpleBox(const QRect& box);
    static std::vector<VertexData>  createAspectRatioBox(double ratio);

   static bool is_inside(float x, float y, const std::vector<VertexData>& curve);
   static bool is_inside(float x, float y, const QRect& box);
   static bool findminmax(float& minx, float& miny, float& maxx, float& maxy, const std::vector<VertexData>& pts);

   // camera related - for both 2D and 3D
   static PickPrimitiveDataback prepare_pick_databack_ortho(float x, float y, float Z, const QMatrix4x4& view, const QMatrix4x4& proj, const QRect& viewport);
   static PickPrimitiveDataback prepare_pick_databack_perspective(float x, float y, float Z, const QMatrix4x4& view, const QMatrix4x4& proj, const QRect& viewport);

   // from Geometric Tools
    static bool IntrRay3Triangle3(const QVector3D& origin, const QVector3D& ray,
                             const QVector3D& p0, const QVector3D& p1, const QVector3D& p2,
                                  QVector3D& bary /* output */, QVector3D& point, double& t);

};


// debug stuff
std::ostream& operator<<( std::ostream& f, const QVector3D& v);
std::ostream& operator<<( std::ostream& f, const QMatrix4x4& v);
std::ostream& operator<<( std::ostream& f, const QRect& r);

#endif // GEOMETRYUTILS_H
