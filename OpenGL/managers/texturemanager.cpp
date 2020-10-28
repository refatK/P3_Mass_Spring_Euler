#include "texturemanager.h"

TextureManager::TextureManager()
{

}



bool TextureManager::addTexture(std::string pname, std::string imgname){

    auto search = m_textures.find(pname);
    if (search != m_textures.end()) {
            return false;
    }

    QImage* img = 0;
    img = new QImage(imgname.c_str());
    QOpenGLTexture* t = new QOpenGLTexture(*img);
    t->setMinificationFilter(QOpenGLTexture::Nearest);
    t->setMagnificationFilter(QOpenGLTexture::Linear);
    t->setWrapMode(QOpenGLTexture::ClampToBorder);

    TextureImage ti;
    ti.m_image = img;
    ti.m_texture = t;

    if(!ti.is_valid()){
        return false;
    }

    m_textures[pname] = ti;
    return true;
}

TextureImage TextureManager::getTexture(std::string pname){

    TextureImage Z;
    auto search = m_textures.find(pname);
    if (search == m_textures.end()) {
            return Z;
    }

    return search->second;
}
