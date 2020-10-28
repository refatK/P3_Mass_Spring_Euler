#ifndef OGL2DWIDGET_H
#define OGL2DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector>


#include "../managers/texturemanager.h"
#include "../managers/shadermanager.h"

#include "../elements/drawelement.h"

#include "../layers/ogllayer.h"


#include "../../MessagePassing/messagemanager.h"

class OGLTWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OGLTWidget(QWidget *parent = 0);
    ~OGLTWidget();

protected:


    // PART 1 --> UI event handling --> ogl2widget.cpp
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event)override;
    virtual void timerEvent(QTimerEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *event)override;
    virtual void keyReleaseEvent(QKeyEvent *event)override;
    virtual void wheelEvent(QWheelEvent *event)override;

    MouseInfo m_mouse;

    // PART 2
    // OpenGL stuff--> ogl2widget2.cpp
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    bool initTextures();
    bool initShaders();

    // my own methods
    std::string getWindowTitle2();

public:
    ShaderManager m_sm;
    TextureManager m_tm;

    int counter=0;


public:


    // orthographic 2d projection
    bool m_button_pressed;
    // temporary storge of the mouse position
    QVector2D mousePressPosition;
    QVector2D prevMouseMovePosition;

    // different layers
    std::vector<OGLLayer*> m_layers;

public:

    void glError();

public:

    bool save_frame_buffer(std::string f);

    // LOAD/SAVE functions --> ogl2widget3.cpp
    void readCurves(QDataStream* in);
    void writeCurves(QDataStream* out);


    void SendMessages(TMessage*);


    // picking and selexcting callback
    virtual void PickPrimitiveGrab(const PickPrimitiveDataback&){}
    virtual void PickPrimitiveDrag(const PickPrimitiveDataback&){}
    virtual void PickPrimitiveRelease(const PickPrimitiveDataback&){}


public slots:


};

#endif // OGLTWIDGET_H
