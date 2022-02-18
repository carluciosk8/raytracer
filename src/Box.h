/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __BOX_H__
#define __BOX_H__


#include "Primitive.h"
#include "MathLib.h"
#include "aabb.h"


class Box : public Primitive
{
	public:
		Box();
		Box(aabb& a_Box);
		~Box();

		virtual int GetType() { return AABB; }
		virtual int Intersect( Ray& a_Ray, double& a_Dist );
		virtual vector3 GetNormal( vector3& );
		virtual void Light(bool a_Light);

		vector3& GetPos() { return m_Box.GetPos(); }
		vector3& GetSize() { return m_Box.GetSize(); }
		double GetGridX( int a_Idx ) { return m_Grid[a_Idx << 1]; }
		double GetGridY( int a_Idx ) { return m_Grid[(a_Idx << 1) + 1]; }
		aabb GetAABB() { return m_Box; }


	protected:
		aabb m_Box;
		double* m_Grid;
};


#endif
