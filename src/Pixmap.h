/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __PIXMAP_H__
#define __PIXMAP_H__


#include <SDL/SDL.h>

#include "Texture.h"
#include "Sphere.h"
#include "Plane.h"
#include "Command.h"

class TexImage : public Texture
{
public:
    TexImage(){}
    TexImage(const char* file, double a_UScale, double a_VScale);
    virtual ~TexImage();

    virtual Color getColor(vector3 pnt, Primitive* prim);
    void SetUVScale(double a_UScale, double a_VScale);


private:
    Color getProjectedSphereTexel(vector3 pnt, PrimSphere* sphere);
    Color getProjectedPlaneTexel(vector3 pnt, PrimPlane* plane);
    Color GetTexel(double a_U, double a_V);
    Color getpixel(int x, int y);

    SDL_Surface*  m_Surface;
    double m_UScale, m_VScale, m_RUScale, m_RVScale;
};

DECLARE_COMMAND(TexImage);



#endif
