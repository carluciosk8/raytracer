/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Primitive.h"



Primitive::Primitive() : m_Light(false), m_RayID(-1)
{
}


void CmdPrimitive::parse(std::istringstream& line, Scene* scene)
{
    std::string sType;
    line >> sType;
    Command::parse(sType, line, scene);
}

