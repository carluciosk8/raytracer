/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Pixmap.h"
#include "Primitive.h"
#include "Scene.h"


TexImage::TexImage(const char *file, double a_UScale, double a_VScale):
    m_UScale(1.0f),
    m_VScale(1.0f),
    m_RUScale(1.0f),
    m_RVScale(1.0f),
    m_Surface(NULL)
{
    m_Surface = SDL_LoadBMP(file);
    if (!m_Surface)
    {
        printf("Runtime error: Pixmap::Pixmap(%s) - File not found\n", file);
        exit(1);
    }

    SetUVScale(a_UScale, a_VScale);
}



TexImage::~TexImage()
{
    SDL_FreeSurface(m_Surface);
}



void CmdTexImage::parse(std::istringstream& line, Scene* scene)
{
    string file;
    double UScale;
    double VScale;

    line >> file >> UScale >> VScale;

    Texture* opTexture = new TexImage(file.c_str(), UScale, VScale);
    scene->addTexture(opTexture);
}



void TexImage::SetUVScale(double a_UScale, double a_VScale)
{
    m_UScale = a_UScale;
    m_VScale = a_VScale;
    m_RUScale = 1.0f / a_UScale;
    m_RVScale = 1.0f / a_VScale;
}



Color TexImage::getColor(vector3 pnt, Primitive* prim)
{
    switch (prim->GetType())
    {
        case Primitive::AABB:
        {
            return Color(0,0,0);
        }
        break;

        case Primitive::PLANE:
        {
            return getProjectedPlaneTexel(pnt, (PrimPlane*)prim);
        }
        break;

        case Primitive::SPHERE:
        {
            return getProjectedSphereTexel(pnt, (PrimSphere*)prim);
        }
        break;

        default:
        {
            return Color(0,0,0);
        }
    }
}



Color TexImage::getProjectedSphereTexel(vector3 pnt, PrimSphere* sphere)
{
    Color retval;
    vector3 vp = (pnt - sphere->GetCentre()) * sphere->GetRRadius();
    double phi = acos( -DOT( vp, sphere->getVn() ) );
    double u, v = phi * m_RVScale * (1.0f / PI);
    double theta = (acos( DOT( sphere->getVe(), vp ) / sin( phi ))) * (2.0f / PI);
    if (DOT( sphere->getVc(), vp ) >= 0)
        u = (1.0f - theta) * m_RUScale;
    else
        u = theta * m_RUScale;
    retval = GetTexel( u, v );

    return retval;
}



Color TexImage::getProjectedPlaneTexel(vector3 pnt, PrimPlane* plane)
{
    return GetTexel( m_UScale*DOT(pnt, plane->getUAxis()) , m_VScale*DOT(pnt, plane->getVAxis()) );
}



Color TexImage::GetTexel(double a_U, double a_V)
{
    // fetch a bilinearly filtered texel
    double fu = abs((a_U + 1000.0f) * m_Surface->w);
    double fv = abs((a_V + 1000.0f) * m_Surface->h);
    int u1 = ((int)fu) % m_Surface->w;
    int v1 = ((int)fv) % m_Surface->h;
    int u2 = (u1 + 1) % m_Surface->w;
    int v2 = (v1 + 1) % m_Surface->h;
    // calculate fractional parts of u and v
    double fracu = fu - floor( fu );
    double fracv = fv - floor( fv );
    // calculate weight factors
    double w1 = (1 - fracu) * (1 - fracv);
    double w2 = fracu * (1 - fracv);
    double w3 = (1 - fracu) * fracv;
    double w4 = fracu *  fracv;
    // fetch four texels
    Color c1 = getpixel(u1, v1);
    Color c2 = getpixel(u2, v1);
    Color c3 = getpixel(u1, v2);
    Color c4 = getpixel(u2, v2);
    // scale and sum the four colors
    return c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
}



Color TexImage::getpixel(int x, int y)
{
    Uint32  c;
    int  bpp = m_Surface->format->BytesPerPixel;
    Uint8*  p = (Uint8 *)m_Surface->pixels + y * m_Surface->pitch + x * bpp;

    switch(bpp)
    {
        case 1:
            c = *p;
        break;

        case 2:
            c = *(Uint16 *)p;
        break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                c = p[0] << 16 | p[1] << 8 | p[2];
            else
                c = p[0] | p[1] << 8 | p[2] << 16;
        break;

        case 4:
            c = *(Uint32 *)p;
        break;

        default:
            c = 0;
        break;
    }

    Uint8 r,g,b;
    SDL_GetRGB(c, m_Surface->format, &r, &g, &b);

    return Color(double(r)/255.0, double(g)/255.0, double(b)/255.0);
}
