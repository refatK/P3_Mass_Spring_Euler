#ifndef A2_2DWIDGET_H
#define A2_2DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>

#include <OpenGL/widgets/oglTwidget.h>



class A2_2DWidget : public OGLTWidget
{
    Q_OBJECT

public:
    explicit A2_2DWidget(QWidget *parent = 0);
    ~A2_2DWidget();

    QTimer* timer;
    QElapsedTimer* time;

protected:


    // PART 1 --> UI event handling --> ogl2widget.cpp
  //  void mousePressEvent(QMouseEvent *e) override;
  //  void mouseReleaseEvent(QMouseEvent *e) override;
  //  void mouseMoveEvent(QMouseEvent *event) override;
   // void timerEvent(QTimerEvent *e) override;
    void keyPressEvent(QKeyEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;
 //   void wheelEvent(QWheelEvent *event)override;


    MouseInfo m_mouse;


    // PART 2
    // OpenGL stuff--> ogl2widget2.cpp
    // my own methods



    //for later
public slots:

    void update_physics();

};

#endif // OGLTWIDGET_H
