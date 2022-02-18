/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <cassert>
#include "Plane.h"
#include "Scene.h"



PrimPlane::PrimPlane( vector3& a_Normal, double a_D ) :
    m_Plane( plane( a_Normal, a_D ) )
{
    m_UAxis = vector3( m_Plane.N.y, m_Plane.N.z, -m_Plane.N.x );
    m_VAxis = m_UAxis.Cross( m_Plane.N );
}



PrimPlane::~PrimPlane()
{
}



int PrimPlane::Intersect( Ray& a_Ray, double& a_Dist )
{
    double d = DOT( m_Plane.N, a_Ray.GetDirection() );
    if (d < 0)
    {
        a_Dist = -(DOT( m_Plane.N, a_Ray.GetOrigin() ) + m_Plane.D) / d;
        return HIT;
    }
    return MISS;
}



void CmdPrimPlane::parse(std::istringstream& line, Scene* scene)
{
    int iColorIdx, iMaterialIdx;
    vector3 N;
    double d;

    read_number(line, iColorIdx);
    read_number(line, iMaterialIdx);
    read_vector3(line, N);
    read_number(line, d);

    Primitive* opPrimitive = new PrimPlane(N, d);
    opPrimitive->SetMaterial(scene->getMaterial(iMaterialIdx));
    opPrimitive->SetTexture(scene->getTexture(iColorIdx));
    //std::cout << "Plane: Normal = (" << N.x << ", " << N.y << ", " << N.z << ") , d = " << d << std::endl;
    scene->addPrimitive(opPrimitive);
}
