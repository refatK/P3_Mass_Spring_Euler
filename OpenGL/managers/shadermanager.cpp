#include "shadermanager.h"

#include <iostream>

using namespace std;

ShaderManager::ShaderManager()
{

}


bool ShaderManager::addProgram(std::string pname,std::string vertex, std::string fragment){

    auto search = m_programs.find(pname);
    if (search != m_programs.end()) {
            return false;
    }

    QOpenGLShaderProgram* program = new QOpenGLShaderProgram;

    // Compile vertex shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
        return false;

    // Compile fragment shader
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str()))
        return false;

    // Link shader pipeline
    if (!program->link())
       return false;

    // Bind shader pipeline for use
    if (!program->bind())
        return false;

   // cout<<pname<<endl;
   // std::cout<<program->log().toUtf8().constData()<<endl;

    m_programs[pname] = program;
    return true;

}


QOpenGLShaderProgram* ShaderManager::getProgram(std::string pname){

     auto search = m_programs.find(pname);
    if (search != m_programs.end()) {
            return search->second;
    }

    return 0;
}
