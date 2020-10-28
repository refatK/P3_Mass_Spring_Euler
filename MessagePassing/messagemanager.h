#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <string>
#include <QVector2D>
#include <QVector3D>

class TMessage {
public:
    TMessage(){


    }

    std::string m_attribute;
    int m_valueI;
    double m_valueD;
    QVector3D m_valueV;

    bool m_model_message;
    bool m_layer_message;
};


class MessageManager
{
public:
    MessageManager();
};

#endif // MESSAGEMANAGER_H
