/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __SPHERE_H__
#define __SPHERE_H__


#include "Primitive.h"
#include "Command.h"

class PrimSphere : public Primitive
{
public:
    PrimSphere() {}
    PrimSphere(vector3& a_Centre, double a_Radius);
    ~PrimSphere();

    virtual int GetType() { return SPHERE; }
    virtual int Intersect( Ray& a_Ray, double& a_Dist );
    virtual vector3 GetNormal( vector3& a_Pos ) { return (a_Pos - center) * inverse_radius; }

    vector3& GetCentre() { return center; }
    double GetSqRadius() { return squared_radius; }
    double GetRRadius() { return inverse_radius; }
    vector3 getVe() { return m_Ve; }
    vector3 getVn() { return m_Vn; }
    vector3 getVc() { return m_Vc; }


private:
    vector3 center;
    double squared_radius, radius, inverse_radius;
    vector3 m_Ve, m_Vn, m_Vc;
};


class Light : public PrimSphere
{
public:
    Light() {}
};

DECLARE_COMMAND(PrimSphere);
DECLARE_COMMAND(Light);



#endif
