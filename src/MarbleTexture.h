/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __MARBLETEXTURE_H__
#define __MARBLETEXTURE_H__


#define SIZE           64
#define SIZE_1         65


#include "Texture.h"
#include "Command.h"

class TexMarble : public Texture
{
public:
    TexMarble() {}
    TexMarble(Color col1, Color col2, double scale, double factor, double pixelSize);
    virtual ~TexMarble() {}

    virtual Color getColor(vector3 pnt, Primitive* prim);


private:
    Color marble_color(double x);
    double turbulence(vector3 pnt, double pixel_size);
    double calc_noise(vector3 pnt);

    Color  m_Color1;
    Color  m_Color2;
    double  m_Scale;
    double  mFactor;
    double  mPixelSize;
    double  m_NoiseTable[SIZE+1][SIZE+1][SIZE+1];
};

DECLARE_COMMAND(TexMarble);



#endif
