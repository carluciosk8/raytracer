/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <iostream>


#include "Renderer.h"
#include "Scene.h"

#include "Material.h"
#include "Texture.h"
#include "CellTexture.h"
#include "Pixmap.h"
#include "MarbleTexture.h"

#include "Sphere.h"
#include "Box.h"



Renderer::Renderer()
{
    REGISTER_COMMAND(Comment);
    REGISTER_COMMAND(Seed);
    REGISTER_COMMAND(Camera);
    REGISTER_COMMAND(Ambient);

    REGISTER_COMMAND(Material);
    REGISTER_COMMAND(Light);

    REGISTER_COMMAND(Primitive);
    REGISTER_COMMAND(PrimSphere);
    REGISTER_COMMAND(PrimPlane);

    REGISTER_COMMAND(Texture);
    REGISTER_COMMAND(TexSolid);
    REGISTER_COMMAND(TexChecker);
    REGISTER_COMMAND(TexCellular);
    REGISTER_COMMAND(TexImage);
    REGISTER_COMMAND(TexMarble);
}



Renderer::~Renderer()
{
}



void Renderer::SetTarget( Screen* a_Dest, int a_Width, int a_Height )
{
    m_Dest = a_Dest;
    m_Width = a_Width;
    m_Height = a_Height;
}



Primitive* Renderer::Raytrace( Ray& a_Ray, Color& a_Acc, int a_Depth, double a_RIndex, double& a_Dist, double a_Samples, double a_SScale )
{
    if (a_Depth > TRACEDEPTH) return 0;

    a_Dist = 100000000000000.0;
    vector3 pi;
    Primitive* prim = 0;

    // find the nearest intersection
    int result = 0;
    for ( unsigned int s = 0; s < m_Scene->GetNrPrimitives(); s++ )
    {
        Primitive* pr = m_Scene->GetPrimitive( s );
        int res;
        double d;
        if (res = pr->Intersect( a_Ray, d ))
        {
            if ( d < a_Dist )
            {
                a_Dist = d;
                prim = pr;
                result = res; // 0 = miss, 1 = hit, -1 = hit from inside primitive
            }
        }
    }

    if (!result)
    {
        a_Acc += Color(.5, .5, .5);
        return 0;
    }
    else
    {
        // determine color at point of intersection
        pi = a_Ray.GetOrigin() + a_Ray.GetDirection() * a_Dist;
        Color color = prim->GetColor(pi);

        // calculate ambient
        vector3 ambient = prim->GetMaterial()->GetAmbient() * m_Scene->getAmbient();
        a_Acc += ambient;

        // trace lights
        for ( unsigned int l = 0; l < m_Scene->GetNrLights(); l++ )
        {
            Primitive* light = m_Scene->GetLight( l );
            // handle point light source
            vector3 L;
            double shade = CalcShade( prim, light, pi, L, a_Samples, a_SScale );
            if (shade > 0)
            {
                vector3 diffuse(0,0,0);
                vector3 specular(0,0,0);
                double attenuation;

                // calculate diffuse shading
                vector3 N = prim->GetNormal( pi );
                if (prim->GetMaterial()->GetDiffuse() > 0)
                {
                    double dot = DOT( L, N );
                    if (dot > 0)
                    {
                        double diff = dot * prim->GetMaterial()->GetDiffuse() * shade;
                        // add diffuse component to ray color
                        diffuse = diff * color * light->m_LightColor;
                    }
                }
                // determine specular component
                if (prim->GetMaterial()->GetSpecular() > 0)
                {
                    // point light source: sample once for specular highlight
                    vector3 V = a_Ray.GetDirection();
                    vector3 R = L - 2.0f * DOT( L, N ) * N;
                    double dot = DOT( V, R );
                    if (dot > 0)
                    {
                        double spec = pow( dot, prim->GetMaterial()->GetShininess() ) * prim->GetMaterial()->GetSpecular() * shade;
                        // add specular component to ray color
                        specular = spec * light->m_LightColor;
                    }
                }

                // determine distance attenuation
                if (light->GetType() == Primitive::SPHERE)
                {
                    vector3 pl = (((PrimSphere*)light)->GetCentre() - pi);
                    double d = pl.Length();
                    attenuation = 1.0f / (light->m_LightAtt_a + d*light->m_LightAtt_b + d*d*light->m_LightAtt_c);
                }
                else if (light->GetType() == Primitive::AABB)
                {
                    vector3 pl = (((Box*)light)->GetAABB().GetPos() - pi);
                    double d = pl.Length();
                    attenuation = 1.0f / (light->m_LightAtt_a + d*light->m_LightAtt_b + d*d*light->m_LightAtt_c);
                }

                a_Acc += attenuation*(diffuse + specular);
            }
        }
        // calculate reflection
        double refl = prim->GetMaterial()->GetReflection();
        if ((refl > 0.0f) && (a_Depth < TRACEDEPTH))
        {
            // calculate perfect reflection
            vector3 N = prim->GetNormal( pi );
            vector3 R = a_Ray.GetDirection() - 2.0f * DOT( a_Ray.GetDirection(), N ) * N;
            Color rcol( 0, 0, 0 );
            double dist;
            Ray ray( pi + R * EPSILON, R, ++m_CurID );
        #ifdef IMPORTANCE
            Raytrace( ray, rcol, a_Depth + 1, a_RIndex, dist, a_Samples * 0.5f, a_SScale * 2 );
        #else
            Raytrace( ray, rcol, a_Depth + 1, a_RIndex, dist, a_Samples, a_SScale );
        #endif
            a_Acc += refl * rcol * color;
        }

        // calculate refraction
        double refr = prim->GetMaterial()->GetRefraction();
        if ((refr > 0) && (a_Depth < TRACEDEPTH) )
        {
            double rindex = prim->GetMaterial()->GetRefrIndex();
            double n = a_RIndex / rindex;
            vector3 N = prim->GetNormal( pi ) * (double)result;
            double cosI = -DOT( N, a_Ray.GetDirection() );
            double cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
            if (cosT2 > 0.0f)
            {
                vector3 T = (n * a_Ray.GetDirection()) + (n * cosI - sqrt( cosT2 )) * N;
                T.Normalize();
                Color rcol( 0, 0, 0 );
                double dist;
                Ray ray( pi + T * EPSILON, T, ++m_CurID );
            #ifdef IMPORTANCE
                Raytrace( ray, rcol, a_Depth + 1, rindex, dist, a_Samples * 0.5f, a_SScale * 2 );
            #else
                Raytrace( ray, rcol, a_Depth + 1, rindex, dist, a_Samples, a_SScale );
            #endif
                // apply Beer's law
                Color absorbance = prim->GetColor(pi) * 0.15 * -dist;
                Color transparency = Color( exp( absorbance.r ), exp( absorbance.g ), exp( absorbance.b ) );
                a_Acc += refr * rcol * transparency;
            }
        }
    }
    // return pointer to primitive hit by primary ray
    return prim;
}



double Renderer::CalcShade( Primitive* a_Prim, Primitive* a_Light, vector3 a_IP, vector3& a_Dir, double a_Samples, double a_SScale )
{
    double retval;
    Primitive* prim = 0;
    if (a_Light->GetType() == Primitive::SPHERE)
    {
        // handle point light source
        retval = 1.0f;
        a_Dir = ((PrimSphere*)a_Light)->GetCentre() - a_IP;
        double tdist = LENGTH( a_Dir );
        a_Dir *= (1.0f / tdist);
        Ray a_Ray( a_IP + a_Dir * EPSILON, a_Dir, ++m_CurID );
        int result = 0;
        for ( unsigned int s = 0; s < m_Scene->GetNrPrimitives(); s++ )
        {
            Primitive* pr = m_Scene->GetPrimitive( s );
            int res;
            if (pr!=a_Prim)
            {
                double dist;
                if (res = pr->Intersect( a_Ray, dist ))
                {
                    if (dist< tdist)
                    {
                        //prim = pr;
                        result = res; // 0 = miss, 1 = hit, -1 = hit from inside primitive
                    }
                }
            }
        }
        if (result)
            retval = 0;
    }
    else if (a_Light->GetType() == Primitive::AABB)
    {
        retval = 0;
        Box* b = (Box*)a_Light;
        a_Dir = (b->GetPos() + 0.5f * b->GetSize()) - a_IP;
        NORMALIZE( a_Dir );
        // Monte Carlo rendering
        double deltax = b->GetSize().x * 0.25f, deltay = b->GetSize().z * 0.25f;
        for ( int i = 0; i < a_Samples; i++ )
        {
            vector3 lp( b->GetGridX( i & 15 ) + m_Twister.Rand() * deltax, b->GetPos().y, b->GetGridY( i & 15 ) + m_Twister.Rand() * deltay );
            vector3 dir = lp - a_IP;
            double ldist = (double)LENGTH( dir );
            dir *= 1.0f / ldist;
            //if (FindNearest( Ray( a_IP + dir * EPSILON, dir, ++m_CurID ), ldist, prim ))
            //{
            //	if (prim == a_Light) retval += a_SScale;
            //}
            Ray a_Ray( a_IP + dir * EPSILON, dir, ++m_CurID );
            double d = 1000000000000.0;
            for ( unsigned int s = 0; s < m_Scene->GetNrPrimitives(); s++ )
            {
                Primitive* pr = m_Scene->GetPrimitive(s);
                double dist;
                if (pr != a_Prim && pr->Intersect( a_Ray, dist ))
                {
                    if (dist<d)
                    {
                        d=dist;
                        prim = pr;
                    }
                }
            }
            for ( unsigned int s = 0; s < m_Scene->GetNrLights(); s++ )
            {
                Primitive* pr = m_Scene->GetLight( s );
                double dist;
                if (pr->Intersect( a_Ray, dist ))
                {
                    if (dist<d)
                    {
                        d=dist;
                        prim = pr;
                    }
                }
            }
            if (prim == a_Light) retval += a_SScale;
        }
    }
    return retval;
}



void Renderer::InitRender()
{
    // reset ray id counter
    m_CurID = 0;
    // set eye and screen plane position
    double eye_z = .3 / tan( (PI*m_Scene->getCamera().getFoV()/180.0)/2.0);
    m_Origin = vector3( 0, 0, 0 );
    double aspect = static_cast<double>(m_Width) / m_Height;
    m_P1 = vector3(  0.5*aspect,  0.5, eye_z );
    m_P2 = vector3( -0.5*aspect,  0.5, eye_z );
    m_P3 = vector3( -0.5*aspect, -0.5, eye_z );
    m_P4 = vector3(  0.5*aspect, -0.5, eye_z );
    // calculate camera matrix
    vector3 zaxis = m_Scene->getCamera().getLook() - m_Scene->getCamera().getEye();
    zaxis.Normalize();
    vector3 xaxis = m_Scene->getCamera().getUp().Cross( zaxis );
    vector3 yaxis = xaxis.Cross( -zaxis );
    matrix m;
    m.cell[0] = xaxis.x, m.cell[1] = xaxis.y, m.cell[2] = xaxis.z;
    m.cell[4] = yaxis.x, m.cell[5] = yaxis.y, m.cell[6] = yaxis.z;
    m.cell[8] = zaxis.x, m.cell[9] = zaxis.y, m.cell[10] = zaxis.z;
    m.Invert();
    m.cell[3] = m_Scene->getCamera().getEye().x, m.cell[7] = m_Scene->getCamera().getEye().y, m.cell[11] = m_Scene->getCamera().getEye().z;
    // move camera
    m_Origin = m.Transform( m_Origin );
    m_P1 = m.Transform( m_P1 );
    m_P2 = m.Transform( m_P2 );
    m_P3 = m.Transform( m_P3 );
    m_P4 = m.Transform( m_P4 );
    // calculate screen plane interpolation vectors
    m_DX = (m_P2 - m_P1) * (1.0f / m_Width);
    m_DY = (m_P4 - m_P1) * (1.0f / m_Height);
    std::cout << "m_DX: " << m_DX.Length() << std::endl;
    std::cout << "m_DY: " << m_DY.Length() << std::endl;
}



Primitive* Renderer::RenderRay( vector3 a_ScreenPos, Color& a_Acc )
{
    vector3 dir = a_ScreenPos - m_Origin;
    NORMALIZE( dir );
    Ray r( m_Origin, dir, ++m_CurID );
    double dist;
    return Raytrace( r, a_Acc, 0, 1.0f, dist, SAMPLES, 1.0f / SAMPLES );
}



bool Renderer::Render(int start, int step)
{
    for ( int y = start; y < m_Height; y += step )
    {
        vector3 lpos = m_P1 + m_DY*double(y);
        for ( int x = 0; x < m_Width; x++ )
        {
            Color acc(0, 0, 0);
            RenderRay(lpos, acc);

            int r, g, b;

            r = (int)(acc.r * 256);
            g = (int)(acc.g * 256);
            b = (int)(acc.b * 256);

            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            m_Dest->putpixel(x, y, SDL_MapRGB(m_Dest->getSDLSurface()->format, r, g, b));
            lpos += m_DX;
        }

        m_Dest->update();
    }
    return true;
}


int Renderer::RenderP(void* data)
{
    RenderData* renderData = static_cast<RenderData*>(data);
    renderData->engine->Render(renderData->start, renderData->step);
}
