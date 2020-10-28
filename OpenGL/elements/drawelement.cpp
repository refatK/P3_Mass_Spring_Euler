#include "drawelement.h"

#include <QUuid>

#include "../widgets/oglTwidget.h"

DrawElement::DrawElement(OGLTWidget* parent)
    :QOpenGLFunctions(parent->context()),
      m_parent(parent),
      m_bIsInitGL(false),
      m_bUpdateBuffer(true),
      m_is_movable(true),
      m_bHidden(false),
      m_bUI(true),
      m_bPicking(true)
{

    m_id = QUuid::createUuid().toString().toStdString();

}



bool DrawElement::saveVertexData(QDataStream* out, const VertexData& data){

    (*out)<<(data.position);
    (*out)<<(data.texCoord);

    return true;
}

bool DrawElement::loadVertexData(QDataStream* in, VertexData& data){

    // I assume that there are streaming operators for vectors - will see if this is the case
    (*in)>>(data.position);
    (*in)>>(data.texCoord);

    return true;
}
