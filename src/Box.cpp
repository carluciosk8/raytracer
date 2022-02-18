/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Box.h"



Box::Box() : 
	m_Box(vector3(0,0,0), vector3(0,0,0)),
	m_Grid(NULL)
{}



Box::Box(aabb& a_Box) : 
	m_Box(a_Box), 
	m_Grid(NULL) 
{}



Box::~Box()
{
	if (m_Grid)
		delete[] m_Grid;
}



int Box::Intersect(Ray& a_Ray, double& a_Dist)
{
	double dist[6];
	vector3 ip[6], d = a_Ray.GetDirection(), o = a_Ray.GetOrigin();
	bool retval = MISS;
	for ( int i = 0; i < 6; i++ ) dist[i] = -1;
	vector3 v1 = m_Box.GetPos(), v2 = m_Box.GetPos() + GetSize();
	if (d.x) 
	{
		double rc = 1.0f / d.x;
		dist[0] = (v1.x - o.x) * rc;
		dist[3] = (v2.x - o.x) * rc;
	}
	if (d.y) 
	{
		double rc = 1.0f / d.y;
		dist[1] = (v1.y - o.y) * rc;
		dist[4] = (v2.y - o.y) * rc;
	}
	if (d.z) 
	{
		double rc = 1.0f / d.z;
		dist[2] = (v1.z - o.z) * rc;
		dist[5] = (v2.z - o.z) * rc;
	}
	for ( int i = 0; i < 6; i++ ) if (dist[i] > 0)
	{
		ip[i] = o + dist[i] * d;
		if ((ip[i].x > (v1.x - EPSILON)) && (ip[i].x < (v2.x + EPSILON)) && 
			(ip[i].y > (v1.y - EPSILON)) && (ip[i].y < (v2.y + EPSILON)) &&
			(ip[i].z > (v1.z - EPSILON)) && (ip[i].z < (v2.z + EPSILON)))
		{
			a_Dist = dist[i];
			retval = HIT;
		}
	}
	return retval;
}



vector3 Box::GetNormal(vector3& a_Pos)
{
	double dist[6];
	dist[0] = (double)fabs( m_Box.GetSize().x - m_Box.GetPos().x );
	dist[1] = (double)fabs( m_Box.GetSize().x + m_Box.GetSize().x - m_Box.GetPos().x );
	dist[2] = (double)fabs( m_Box.GetSize().y - m_Box.GetPos().y );
	dist[3] = (double)fabs( m_Box.GetSize().y + m_Box.GetSize().y - m_Box.GetPos().y );
	dist[4] = (double)fabs( m_Box.GetSize().z - m_Box.GetPos().z );
	dist[5] = (double)fabs( m_Box.GetSize().z + m_Box.GetSize().z - m_Box.GetPos().z );
	int best = 0;
	double bdist = dist[0];
	for ( int i = 1 ; i < 6; i++ ) if (dist[i] < bdist) 
	{ 
		bdist = dist[i]; 
		best = i;
	}
	if (best == 0) return vector3( -1, 0, 0 );
	else if (best == 1) return vector3( 1, 0, 0 );
	else if (best == 2) return vector3( 0, -1, 0 );
	else if (best == 3)  return vector3( 0, 1, 0 );
	else if (best == 4) return vector3( 0, 0, -1 );
	else return vector3( 0, 0, 1 );
}



void Box::Light(bool a_Light)
{
	m_Light = a_Light;
	if (!m_Grid)
	{
		m_Grid = new double[32];
		m_Grid[ 0] = 1, m_Grid[ 1] = 2;
		m_Grid[ 2] = 3, m_Grid[ 3] = 3;
		m_Grid[ 4] = 2, m_Grid[ 5] = 0;
		m_Grid[ 6] = 0, m_Grid[ 7] = 1;
		m_Grid[ 8] = 2, m_Grid[ 9] = 3;
		m_Grid[10] = 0, m_Grid[11] = 3;
		m_Grid[12] = 0, m_Grid[13] = 0;
		m_Grid[14] = 2, m_Grid[15] = 2;
		m_Grid[16] = 3, m_Grid[17] = 1;
		m_Grid[18] = 1, m_Grid[19] = 3;
		m_Grid[20] = 1, m_Grid[21] = 0;
		m_Grid[22] = 3, m_Grid[23] = 2;
		m_Grid[24] = 2, m_Grid[25] = 1;
		m_Grid[26] = 3, m_Grid[27] = 0;
		m_Grid[28] = 1, m_Grid[29] = 1;
		m_Grid[30] = 0, m_Grid[31] = 2;
		for ( int i = 0; i < 16; i++ )
		{
			m_Grid[i * 2] = m_Grid[i * 2] * m_Box.GetSize().x / 4 + m_Box.GetPos().x;
			m_Grid[i * 2 + 1] = m_Grid[i * 2 + 1] * m_Box.GetSize().z / 4 + m_Box.GetPos().z;
		}
	}
}
