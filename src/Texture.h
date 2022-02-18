/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __TEXTURE_H__
#define __TEXTURE_H__


#include "MathLib.h"
#include "Command.h"

class Primitive;

class Texture
{
    public:
        Texture();
        virtual ~Texture() {}

        virtual Color getColor(vector3 pnt, Primitive* prim){}
};



class TexSolid : public Texture
{
    public:
        TexSolid() : m_Color(0,0,0) {}
        TexSolid(Color& c) : m_Color(c) {}
        virtual ~TexSolid() {}

        virtual Color getColor(vector3 pnt, Primitive* prim) { return m_Color; }


    private:
        Color m_Color;
};



class TexChecker : public Texture
{
    public:
        TexChecker(): m_Color1(0,0,0), m_Color2(1,1,1), m_Size(10) {}
        TexChecker(Color& c1, Color& c2, double s) : m_Color1(c1), m_Color2(c2), m_Size(s) {}
        virtual ~TexChecker() {}

        virtual Color getColor(vector3 pnt, Primitive* prim);


    private:
        Color  m_Color1;
        Color  m_Color2;
        double  m_Size;
};

DECLARE_COMMAND(Texture);
DECLARE_COMMAND(TexSolid);
DECLARE_COMMAND(TexChecker);



#endif
