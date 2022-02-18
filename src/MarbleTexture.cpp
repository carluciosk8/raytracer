/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <cstring>

#include "MarbleTexture.h"
#include "Scene.h"


TexMarble::TexMarble(Color col1, Color col2, double scale, double factor, double pixelSize)
    : m_Color1(col1),
      m_Color2(col2),
      m_Scale(scale),
    mFactor(factor),
    mPixelSize(pixelSize)
{
    long    i,j, k, ii,jj,kk;

    for (i=0; i<SIZE; i++)
        for (j=0; j<SIZE; j++)
            for (k=0; k<SIZE; k++)
            {
                m_NoiseTable[i][j][k] = (double(rand())/double(RAND_MAX));
            }


    for (i=0; i<SIZE+1; i++)
        for (j=0; j<SIZE+1; j++)
            for (k=0; k<SIZE+1; k++)
            {
                ii = (i == SIZE)? 0:  i;
                jj = (j == SIZE)? 0:  j;
                kk = (k == SIZE)? 0:  k;
                m_NoiseTable[i][j][k] = m_NoiseTable[ii][jj][kk];
            }
}



void CmdTexMarble::parse(std::istringstream& line, Scene* scene)
{
    Color vColor1;
    Color vColor2;
    double fScale;
    double fFactor;
    double fPixelSize;

    read_vector3(line, vColor1);
    read_vector3(line, vColor2);
    read_number(line, fScale);
    read_number(line, fFactor);
    read_number(line, fPixelSize);

    printf("TexMarble  %f %f %f   %f %f %f      %f %f %f\n", vColor1.r, vColor1.g, vColor1.b, vColor2.r, vColor2.g, vColor2.b, fScale, fFactor, fPixelSize);
    Texture* opTexture = new TexMarble(vColor1, vColor2, fScale, fFactor, fPixelSize);
    scene->addTexture(opTexture);
}


Color TexMarble::getColor(vector3 pnt, Primitive* prim)
{
    pnt *= m_Scale*pnt;
    double y;
    y = pnt.y + mFactor*turbulence(pnt, mPixelSize);
    y = sin(y*PI);
    return (marble_color(y));
}



Color TexMarble::marble_color(double x)
{
    Color  clr;
    x = sqrt(x+1.0)*.7071;
    clr.g = m_Color1.g  + m_Color2.g*x;
    //x=sqrt(x);
    clr.r = m_Color1.r + m_Color2.r*x;
    //x = sqrt(x+1.0)*.7071;
    clr.b = m_Color1.b + m_Color2.b*x;
    return (clr);
}



double TexMarble::turbulence(vector3 pnt, double pixel_size)
{
    double t, scale;

    t=0;
    for(scale=1.0; scale >pixel_size; scale/=2.0)
    {
        pnt.x = pnt.x/scale; pnt.y = pnt.y/scale; pnt.z = pnt.z/scale;
        t+=calc_noise(pnt)* scale;
    }
    return(t);
}


double TexMarble::calc_noise(vector3 pnt)
{
    double  t1;
    double  p_l,p_l2,    /* value lerped down left side of face1 & face 2 */
            p_r,p_r2,    /* value lerped down left side of face1 & face 2 */
            p_face1,     /* value lerped across face 1 (x-y plane ceil of z) */
            p_face2,     /* value lerped across face 2 (x-y plane floor of z) */
            p_final;     /* value lerped through cube (in z)                  */

    //extern double noise[SIZE_1][SIZE_1][SIZE_1];
    register int      x, y, z,px,py,pz;

    px = (int)pnt.x;
    py = (int)pnt.y;
    pz = (int)pnt.z;
    x = px &(SIZE-1); /* make sure the values are in the table           */
    y = py &(SIZE-1); /* Effectively, replicates the table thought space */
    z = pz &(SIZE-1);

    t1 = pnt.y - py;
    p_l  = m_NoiseTable[x][y][z+1]+t1*(m_NoiseTable[x][y+1][z+1]-m_NoiseTable[x][y][z+1]);
    p_r  = m_NoiseTable[x+1][y][z+1]+t1*(m_NoiseTable[x+1][y+1][z+1]-m_NoiseTable[x+1][y][z+1]);
    p_l2 = m_NoiseTable[x][y][z]+ t1*( m_NoiseTable[x][y+1][z] - m_NoiseTable[x][y][z]);
    p_r2 = m_NoiseTable[x+1][y][z]+ t1*(m_NoiseTable[x+1][y+1][z] - m_NoiseTable[x+1][y][z]);

    t1 = pnt.x - px;
    p_face1 = p_l + t1 * (p_r - p_l);
    p_face2 = p_l2 + t1 * (p_r2 -p_l2);

    t1 = pnt.z - pz;
    p_final =  p_face2 + t1*(p_face1 -p_face2);

    return(p_final);
}


