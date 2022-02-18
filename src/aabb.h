/***************************************************************************
    Original code written in: Jun 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __AABB_H__
#define __AABB_H__


class aabb
{
    public:
        aabb() : m_Pos( vector3( 0, 0, 0 ) ), m_Size( vector3( 0, 0, 0 ) ) {}
        aabb( const vector3& a_Pos, const vector3& a_Size ) : m_Pos( a_Pos ), m_Size( a_Size ) {}
        vector3& GetPos() { return m_Pos; }
        vector3& GetSize() { return m_Size; }


    private:
        vector3 m_Pos, m_Size;
};



#endif
