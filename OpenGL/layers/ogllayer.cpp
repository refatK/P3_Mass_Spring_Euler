#include "ogllayer.h"


OGLLayer::OGLLayer(OGLTWidget* parent)
    :m_parent(parent),m_bIsInitGL(false), m_bHidden(false), m_bUI(true), m_bPicking(true)
{
    setUIMode(0);
}
