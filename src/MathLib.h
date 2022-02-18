/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __MATHLIB_H__
#define __MATHLIB_H__


#include <cmath>
#include <cstdlib>



inline double Rand( double a_Range ) { return ((double)rand() / RAND_MAX) * a_Range; }


#define DOT(A,B)		(A.x*B.x+A.y*B.y+A.z*B.z)
#define NORMALIZE(A)		{double l=1/sqrt(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
#define LENGTH(A)		(sqrt(A.x*A.x+A.y*A.y+A.z*A.z))
#define SQRLENGTH(A)		(A.x*A.x+A.y*A.y+A.z*A.z)
#define SQRDISTANCE(A,B)	((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))

#define SAMPLES			128
#define EPSILON			0.1
#define TRACEDEPTH		10
#define IMPORTANCE

#define PI			3.141592653589793238462f



class vector3
{
    public:
        vector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
        vector3( double a_X, double a_Y, double a_Z ) : x( a_X ), y( a_Y ), z( a_Z ) {}
        void Set( double a_X, double a_Y, double a_Z ) { x = a_X; y = a_Y; z = a_Z; }
        void Normalize() { double l = 1.0f / Length(); x *= l; y *= l; z *= l; }
        double Length() { return (double)sqrt( x * x + y * y + z * z ); }
        double SqrLength() { return x * x + y * y + z * z; }
        double Dot( vector3 a_V ) { return x * a_V.x + y * a_V.y + z * a_V.z; }
        vector3 Cross( vector3 b ) { return vector3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x ); }
        void operator += ( const vector3& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
        void operator += ( const vector3* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
        void operator -= ( const vector3& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
        void operator -= ( const vector3* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
        void operator *= ( double f ) { x *= f; y *= f; z *= f; }
        void operator *= ( const vector3& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }
        void operator *= ( const vector3* a_V ) { x *= a_V->x; y *= a_V->y; z *= a_V->z; }
        vector3 operator- () const { return vector3( -x, -y, -z ); }
        friend vector3 operator + ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
        friend vector3 operator - ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
        friend vector3 operator + ( const vector3& v1, vector3* v2 ) { return vector3( v1.x + v2->x, v1.y + v2->y, v1.z + v2->z ); }
        friend vector3 operator - ( const vector3& v1, vector3* v2 ) { return vector3( v1.x - v2->x, v1.y - v2->y, v1.z - v2->z ); }
        friend vector3 operator * ( const vector3& v, double f ) { return vector3( v.x * f, v.y * f, v.z * f ); }
        friend vector3 operator * ( const vector3& v1, vector3& v2 ) { return vector3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
        friend vector3 operator * ( double f, const vector3& v ) { return vector3( v.x * f, v.y * f, v.z * f ); }
        friend vector3 operator / ( double f, const vector3& v ) { return vector3( f/v.x, f/v.y, f/v.z ); }
        union
        {
            struct { double x, y, z; };
            struct { double r, g, b; };
            struct { double cell[3]; };
        };
};

class matrix
{
public:
    enum
    {
        TX=3,
        TY=7,
        TZ=11,
        D0=0, D1=5, D2=10, D3=15,
        SX=D0, SY=D1, SZ=D2,
        W=D3
    };
    matrix() { Identity(); }
    void Identity()
    {
        cell[1] = cell[2] = cell[TX] = cell[4] = cell[6] = cell[TY] =
        cell[8] = cell[9] = cell[TZ] = cell[12] = cell[13] = cell[14] = 0;
        cell[D0] = cell[D1] = cell[D2] = cell[W] = 1;
    }
    void Rotate( vector3 a_Pos, double a_RX, double a_RY, double a_RZ )
    {
        matrix t;
        t.RotateX( a_RZ );
        RotateY( a_RY );
        Concatenate( t );
        t.RotateZ( a_RX );
        Concatenate( t );
        Translate( a_Pos );
    }
    void RotateX( double a_RX )
    {
        double sx = (double)sin( a_RX * PI / 180 );
        double cx = (double)cos( a_RX * PI / 180 );
        Identity();
        cell[5] = cx, cell[6] = sx, cell[9] = -sx, cell[10] = cx;
    }
    void RotateY( double a_RY )
    {
        double sy = (double)sin( a_RY * PI / 180 );
        double cy = (double)cos( a_RY * PI / 180 );
        Identity ();
        cell[0] = cy, cell[2] = -sy, cell[8] = sy, cell[10] = cy;
    }
    void RotateZ( double a_RZ )
    {
        double sz = (double)sin( a_RZ * PI / 180 );
        double cz = (double)cos( a_RZ * PI / 180 );
        Identity ();
        cell[0] = cz, cell[1] = sz, cell[4] = -sz, cell[5] = cz;
    }
    void Translate( vector3 a_Pos ) { cell[TX] += a_Pos.x; cell[TY] += a_Pos.y; cell[TZ] += a_Pos.z; }
    void Concatenate( matrix& m2 )
    {
        matrix res;
        for ( int c = 0; c < 4; c++ ) for ( int r = 0; r < 4; r++ )
            res.cell[r * 4 + c] = cell[r * 4] * m2.cell[c] +
                                  cell[r * 4 + 1] * m2.cell[c + 4] +
                                  cell[r * 4 + 2] * m2.cell[c + 8] +
                                  cell[r * 4 + 3] * m2.cell[c + 12];
        for ( int c = 0; c < 16; c++ ) cell[c] = res.cell[c];
    }
    vector3 Transform( vector3& v )
    {
        double x  = cell[0] * v.x + cell[1] * v.y + cell[2] * v.z + cell[3];
        double y  = cell[4] * v.x + cell[5] * v.y + cell[6] * v.z + cell[7];
        double z  = cell[8] * v.x + cell[9] * v.y + cell[10] * v.z + cell[11];
        return vector3( x, y, z );
    }
    void Invert()
    {
        matrix t;
        double tx = -cell[3], ty = -cell[7], tz = -cell[11];
        for ( int h = 0; h < 3; h++ ) for ( int v = 0; v < 3; v++ ) t.cell[h + v * 4] = cell[v + h * 4];
        for ( int i = 0; i < 11; i++ ) cell[i] = t.cell[i];
        cell[3] = tx * cell[0] + ty * cell[1] + tz * cell[2];
        cell[7] = tx * cell[4] + ty * cell[5] + tz * cell[6];
        cell[11] = tx * cell[8] + ty * cell[9] + tz * cell[10];
    }
    double cell[16];
};


typedef struct
{
    vector3 N;
    double D;
} pln;


class plane
{
public:
    plane() : N( 0, 0, 0 ), D( 0 ) {}
    plane( vector3 a_Normal, double a_D ) : N( a_Normal ), D( a_D ) {}
    vector3 N;
    double D;
};



class Ray
{
    public:
        Ray() : m_Origin(vector3(0,0,0)), m_Direction(vector3(0,0,0)), m_ID(0) {}
        Ray(const vector3& a_Origin, const vector3& a_Dir, int a_ID) : m_Origin(a_Origin), m_Direction(a_Dir), m_ID(a_ID) {}

        void SetOrigin( const vector3& a_Origin ) { m_Origin = a_Origin; }
        void SetDirection( const vector3& a_Direction ) { m_Direction = a_Direction; }
        void SetID( int a_ID ) { m_ID = a_ID; }

        vector3& GetOrigin() { return m_Origin; }
        vector3& GetDirection() { return m_Direction; }
        int GetID() { return m_ID; }


    private:
        vector3 m_Origin;
        vector3 m_Direction;
        int m_ID;
};



typedef vector3 Color;



#endif
