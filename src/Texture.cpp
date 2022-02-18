/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Texture.h"
#include "Scene.h"


Texture::Texture()
{
}

Color TexChecker::getColor(vector3 pnt, Primitive* prim)
{
    int x = int((pnt.x+1000000000) / m_Size);
    int y = int((pnt.y+1000000000) / m_Size);
    int z = int((pnt.z+1000000000) / m_Size);

    if ( ((x+y+z) % 2) == 0  )
        return m_Color1;
    else
        return m_Color2;
}


void CmdTexChecker::parse(std::istringstream& line, Scene* scene)
{
    Color vColor1;
    Color vColor2;
    double fSize;

    read_vector3(line, vColor1);
    read_vector3(line, vColor2);
    read_number(line, fSize);

    Texture* opTexture = new TexChecker(vColor1, vColor2, fSize);
    scene->addTexture(opTexture);
}


void CmdTexture::parse(std::istringstream& line, Scene* scene)
{
    string sType;
    line >> sType;
    Command::parse(sType, line, scene);
}


void CmdTexSolid::parse(std::istringstream& line, Scene* scene)
{
    Color vColor;
    std::string str;

    read_vector3(line, vColor);

    Texture* opTexture = new TexSolid(vColor);
    scene->addTexture(opTexture);
}
