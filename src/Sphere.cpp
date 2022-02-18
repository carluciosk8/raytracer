/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <cassert>

#include "Sphere.h"
#include "Scene.h"



PrimSphere::PrimSphere( vector3& a_Centre, double a_Radius ) :
    center(a_Centre),
    squared_radius(a_Radius * a_Radius),
    radius(a_Radius),
    inverse_radius(1.0f / a_Radius)
{
    m_Vn = vector3(0, 1, 0);
    m_Ve = vector3(1, 0, 0);
    m_Vc = m_Vn.Cross(m_Ve);
}



PrimSphere::~PrimSphere()
{
}



int PrimSphere::Intersect( Ray& a_Ray, double& a_Dist )
{
    vector3 v = a_Ray.GetOrigin() - center;
    double b = -DOT( v, a_Ray.GetDirection() );
    double det = (b * b) - DOT( v, v ) + squared_radius;
    int retval = MISS;
    if (det > 0)
    {
        det = sqrt( det );
        double i1 = b - det;
        double i2 = b + det;
        if (i1>0 && i2>0)
        {
            if (i1>i2)
            {
                a_Dist = i2;
                retval = HIT;
            }
            else
            {
                a_Dist = i1;
                retval = HIT;
            }
        }
        else if (i1>0 && i2<=0)
        {
            a_Dist = i1;
            retval = INPRIM;
        }
        else if (i2>0 && i1<=0)
        {
            a_Dist = i2;
            retval = INPRIM;
        }
    }
    return retval;
}



void CmdPrimSphere::parse(std::istringstream& line, Scene* scene)
{
    int iColorIdx, iMaterialIdx;
    vector3 C;
    double r;

    read_number(line, iColorIdx);
    read_number(line, iMaterialIdx);
    read_vector3(line, C);
    read_number(line, r);

    Primitive* opPrimitive = new PrimSphere(C, r);
    opPrimitive->SetMaterial(scene->getMaterial(iMaterialIdx));
    opPrimitive->SetTexture(scene->getTexture(iColorIdx));
    //std::cout << "Sphere:  center = (" << C.x << ", " << C.y << ", " << C.z << ") , radius = " << r << std::endl;
    scene->addPrimitive(opPrimitive);
}



void CmdLight::parse(std::istringstream& line, Scene* scene)
{
    Primitive* light;
    vector3    position;
    vector3    color;
    double     a,b,c;

    read_vector3(line, position);
    read_vector3(line, color);
    read_number(line, a);
    read_number(line, b);
    read_number(line, c);

    light = new PrimSphere(position, 0.000001);
    light->Light(true);
    light->m_LightColor = color;
    light->m_LightAtt_a = a;
    light->m_LightAtt_b = b;
    light->m_LightAtt_c = c;

    scene->addLight(light);
}
