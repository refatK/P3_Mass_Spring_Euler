#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QOpenGLTexture>
#include <map>

class TextureImage{
public:
    QOpenGLTexture* m_texture;
    QImage* m_image;

    TextureImage()
        :m_texture(0), m_image(0){}

    double getAspectRatio()const{
        return (double)m_image->width()/(double)m_image->height();
    }

    bool is_valid()const{
        if(!m_image || !m_texture)
            return false;

        if(m_image->width()==0 || m_image->height()==0)
            return false;

        return true;
    }

};

class TextureManager
{
public:
    TextureManager();

    std::map<std::string, TextureImage> m_textures;
    bool addTexture(std::string pname, std::string imgname);

    TextureImage getTexture(std::string pname);
};

#endif // TEXTUREMANAGER_H
