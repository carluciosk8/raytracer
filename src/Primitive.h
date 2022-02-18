/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__


#include <string>

using std::string;

#include "MathLib.h"
#include "Material.h"
#include "Texture.h"
#include "Command.h"


#define HIT		 1		// Ray hit primitive
#define MISS	 0		// Ray missed primitive
#define INPRIM	 -1		// Ray started inside primitive

class Primitive
{
    public:
        enum
        {
            SPHERE = 1,
            PLANE,
            AABB
        };
        Primitive();// : m_Light(false), m_RayID(-1) {}

        virtual int GetType(){};
        virtual int Intersect( Ray& a_Ray, double& a_Dist ){}
        virtual vector3 GetNormal( vector3& a_Pos ){}
        virtual void Light(bool a_Light) { m_Light = a_Light; }

        Color GetColor( vector3& p ) { return m_Texture->getColor(p, this);	}
        Material* GetMaterial() { return m_Material; }
        Texture* GetTexture() { return m_Texture; }
        void SetMaterial( Material* a_Mat ) { m_Material = a_Mat; }
        void SetTexture( Texture* tex ) { m_Texture = tex; }
        bool IsLight() { return m_Light; }


        vector3  m_LightColor;
        double  m_LightAtt_a;
        double  m_LightAtt_b;
        double  m_LightAtt_c;


    protected:
        Material*  m_Material;
        Texture*  m_Texture;
        bool  m_Light;
        int  m_RayID;
};


DECLARE_COMMAND(Primitive);


#endif
