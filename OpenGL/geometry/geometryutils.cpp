#include "geometryutils.h"

#include <QMatrix>
#include <QGenericMatrix>

using namespace std;

GeometryUtils::GeometryUtils()
{

}

std::vector<VertexData>  GeometryUtils::createSimpleBox(const QRect& box){

    double gscale = 1.0;
    QSysInfo info;
    if(info.productType()=="osx"){
        gscale = 1.0;//global_scale;
    }

       std::vector<VertexData> simple;

       VertexData v1, v2, v3, v4;
       v1.position = QVector3D(gscale * box.x(), gscale * box.y(),  0.0f);
       v1.texCoord = QVector2D(0.0f, 0.0f);

       v2.position = QVector3D( gscale * box.x(), gscale * (box.y() + box.height()),  0.0f);
       v2.texCoord = QVector2D(0.0f, 0.0f);

       v3.position = QVector3D( gscale * (box.x() + box.width()), gscale * (box.y() + box.height()),  0.0f);
       v3.texCoord = QVector2D(0.0f, 0.0f);

       v4.position = QVector3D( gscale * (box.x() + box.width()), gscale * box.y(),  0.0f);
       v4.texCoord = QVector2D(0.0f, 0.0f);


       simple.push_back(v1);
       simple.push_back(v2);
       simple.push_back(v3);
       simple.push_back(v4);

   return simple;



}


std::vector<VertexData>  GeometryUtils::createAspectRatioBox(double ratio){

    std::vector<VertexData> simple;
#if 0

   if(ratio==0){
       return createSimpleBox();

   }

   QRect box = geometry();


   VertexData v1, v2, v3, v4;

   if((double)box.width()<=(double)box.height()*ratio){
       ratio = 1.0 / ratio;

       v1.position = QVector3D(0, 0,  0.0f);
       v1.texCoord = QVector2D(0.0f, 0.0f);

       v2.position = QVector3D( 0, (double)box.width()/ratio,  0.0f);
       v2.texCoord = QVector2D(0.0f, 0.0f);

       v3.position = QVector3D( box.width(), (double)box.width()/ratio,  0.0f);
       v3.texCoord = QVector2D(0.0f, 0.0f);

       v4.position = QVector3D( box.width(), 0,  0.0f);
       v4.texCoord = QVector2D(0.0f, 0.0f);
   } else {

       v1.position = QVector3D(0, 0,  0.0f);
       v1.texCoord = QVector2D(0.0f, 0.0f);

       v2.position = QVector3D( 0, box.height(),  0.0f);
       v2.texCoord = QVector2D(0.0f, 0.0f);

       v3.position = QVector3D( (double)box.height()*ratio, box.height(),  0.0f);
       v3.texCoord = QVector2D(0.0f, 0.0f);

       v4.position = QVector3D( (double)box.width()*ratio, 0,  0.0f);
       v4.texCoord = QVector2D(0.0f, 0.0f);
   }

   simple.push_back(v1);
   simple.push_back(v2);
   simple.push_back(v3);
   simple.push_back(v4);
#endif
  return simple;
}


bool GeometryUtils::findminmax(float& minx, float& miny, float& maxx, float& maxy, const std::vector<VertexData>& pts){
   if(pts.size()==0){
       return false;
   }

   minx = maxx = pts[0].position[0];
   miny = maxy = pts[0].position[1];

   for(int i=1;i<pts.size();++i){
       if(minx>pts[i].position[0])
           minx = pts[i].position[0];
       if(maxx<pts[i].position[0])
           maxx = pts[i].position[0];
       if(miny>pts[i].position[1])
           miny = pts[i].position[1];
       if(maxy<pts[i].position[1])
           maxy = pts[i].position[1];

   }

   return true;
}

bool GeometryUtils::is_inside(float x, float y, const std::vector<VertexData>& curve){


   float minx, miny, maxx, maxy;

   if(!findminmax(minx, miny, maxx, maxy, curve)){
       return false;
   }

   if(x>=minx && x<=maxx && y>=miny && y<=maxy)
       return true;

   return false;

}


bool GeometryUtils::is_inside(float x, float y, const QRect& box){


   float minx = box.left();
   float miny = box.top();
   float maxx = box.right();
   float maxy = box.bottom();


   if(x>=minx && x<=maxx && y>=miny && y<=maxy)
       return true;

   return false;

}



// camera stuff

PickPrimitiveDataback GeometryUtils::prepare_pick_databack_ortho(float x, float y, float Z,
                                                           const QMatrix4x4& view,
                                                           const QMatrix4x4& proj,
                                                           const QRect& viewport){

    PickPrimitiveDataback ret;

    ret.m_mouse_pos[0] = x;
    ret.m_mouse_pos[0] = y;


    // easiest
    y = (float) viewport.height() - y + (float)viewport.top();

    // scale x, y in normalize device coordinates
    x = 2 * (x-(float)viewport.left()) / (float) viewport.width() - 1.0;
    y = 2 * (y-(float)viewport.top()) / (float) viewport.height() - 1.0;



   // QMatrix4x4 proj_inverse = proj.inverted();
    //QVector4D
    QMatrix4x4 A;
    A.setToIdentity();
    A(0, 0) = proj(0,0) - x * proj(2, 0);
    A(0, 1) = proj(0,1) - x * proj(2, 1);

    A(1, 0) = proj(1,0) - y * proj(2, 0);
    A(1, 1) = proj(1,1) - y * proj(2, 1);

   // cout<<"A: "<<A<<endl;
   // cout<<"B: "<<proj<<endl;
   // cout<<"C: "<<view<<endl;


    QVector4D b(x*(proj(2, 2) * Z + proj(2, 3)) - (proj(0,2) * Z + proj(0,3)),
                y*(proj(2, 2) * Z + proj(2, 3)) - (proj(1,2) * Z + proj(1,3)),
                0, 0
                );

     QVector4D pos3dproxy = A.inverted() * b;
     QVector4D pos4d(pos3dproxy[0], pos3dproxy[1], 0, 1);

     pos4d = view.inverted() * pos4d;

     //cout<<"P3: "<<pos4d[0]<<" "<<pos4d[1]<<endl;

     // 2D
     ret.m_dir = QVector3D(pos4d[0], pos4d[1], 0);


     return ret;
}


PickPrimitiveDataback GeometryUtils::prepare_pick_databack_perspective(float x, float y, float Z,
                                                           const QMatrix4x4& view,
                                                           const QMatrix4x4& proj,
                                                           const QRect& viewport){

    PickPrimitiveDataback ret;

    // easiest
    y = (float) viewport.height() - y + (float)viewport.top();

    // scale x, y in normalize device coordinates
    x = 2 * (x-(float)viewport.left()) / (float) viewport.width() - 1.0;
    y = 2 * (y-(float)viewport.top()) / (float) viewport.height() - 1.0;



   // QMatrix4x4 proj_inverse = proj.inverted();
    //QVector4D
    QMatrix4x4 A;
    A.setToIdentity();
    A(0, 0) = proj(0,0) - x * proj(2, 0);
    A(0, 1) = proj(0,1) - x * proj(2, 1);

    A(1, 0) = proj(1,0) - y * proj(2, 0);
    A(1, 1) = proj(1,1) - y * proj(2, 1);

   // cout<<"A: "<<A<<endl;
   // cout<<"B: "<<proj<<endl;
    cout<<"C: "<<view<<endl;


    QVector4D b(x*(proj(2, 2) * Z + proj(2, 3)) - (proj(0,2) * Z + proj(0,3)),
                y*(proj(2, 2) * Z + proj(2, 3)) - (proj(1,2) * Z + proj(1,3)),
                0, 0
                );

     QVector4D pos3dproxy = A.inverted() * b;
     QVector4D ZERO(0,0,0,1);
     QVector4D pos4d(pos3dproxy[0], pos3dproxy[1], Z, 1);

     pos4d = view.inverted() * pos4d;
     QVector4D camera = view.inverted() * ZERO;

     ret.m_origin = QVector3D(camera[0], camera[1], camera[2]);
     QVector3D pos3d(pos4d[0], pos4d[1], pos4d[2]);

     // cout<<"P3: "<<pos4d[0]<<" "<<pos4d[1]<<endl;

     // 2D
     ret.m_dir = pos3d - ret.m_origin;
     cout<<"Ray b4 normalization: "<<ret.m_dir<<endl;
     ret.m_dir.normalize();

     cout<<"Picking info:"<<endl;
     cout<<"Origin: "<<ret.m_origin<<endl;
     cout<<"Ray: "<<ret.m_dir<<endl;

     return ret;
}


// debug stuff

std::ostream& operator<<( std::ostream& f, const QVector3D& v){
    f<<v[0]<<" "<<v[1]<<" "<<v[2];
    return f;
}

std::ostream& operator<<( std::ostream& f, const QMatrix4x4& m){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
           f<<m(i, j)<<" ";
        }
        f<<endl;
    }
    return f;
}


std::ostream& operator<<( std::ostream& f, const QRect& r){

    f<<r.left()<<" "<<r.right()<<" "<<r.top()<<" "<<r.bottom()<<" "<<r.width()<<" "<<r.height()<<endl;

    return f;
}
