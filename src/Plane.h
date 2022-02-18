/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __PLANE_H__
#define __PLANE_H__


#include "Primitive.h"
#include "Command.h"

class PrimPlane : public Primitive
{
public:
    PrimPlane(){}
    PrimPlane( vector3& a_Normal, double a_D );
    ~PrimPlane();

    virtual int GetType() { return PLANE; }
    virtual int Intersect( Ray& a_Ray, double& a_Dist );
    virtual vector3 GetNormal(vector3& a_Pos) { return m_Plane.N; }

    double GetD() { return m_Plane.D; }
    vector3 getUAxis() { return m_UAxis; }
    vector3 getVAxis() { return m_VAxis; }


private:
    plane m_Plane;
    vector3 m_UAxis, m_VAxis;
};


DECLARE_COMMAND(PrimPlane);


#endif
