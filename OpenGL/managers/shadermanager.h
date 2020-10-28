#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QOpenGLShaderProgram>
#include <map>

class ShaderManager
{
public:
    ShaderManager();

    std::map<std::string, QOpenGLShaderProgram*> m_programs;
    bool addProgram(std::string pname, std::string vertex, std::string fragment);

    QOpenGLShaderProgram* getProgram(std::string pname);
};

#endif // SHADERMANAGER_H
