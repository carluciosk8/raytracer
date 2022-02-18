/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __ENGINE_H__
#define __ENGINE_H__


#include "MathLib.h"
#include "Twister.h"
#include "Screen.h"

class Scene;
class Primitive;

class Renderer
{
public:
    Renderer();
   ~Renderer();

    void InitRender();
    void SetTarget( Screen* a_Dest, int a_Width, int a_Height );
    bool Render(int start, int step);
    double CalcShade( Primitive* a_Prim, Primitive* a_Light, vector3 a_IP, vector3& a_Dir, double a_Samples, double a_SScale );
    Primitive* Raytrace( Ray& a_Ray, Color& a_Acc, int a_Depth, double a_RIndex, double& a_Dist, double a_Samples, double a_SScale );
    Primitive* RenderRay( vector3 a_ScreenPos, Color& a_Acc );
    inline Scene* GetScene() { return m_Scene; }
    inline void setScene(Scene* opScene) { m_Scene = opScene; }

    static int RenderP(void* data);

protected:
    Scene* m_Scene;
    Screen* m_Dest;
    int m_Width, m_Height;
    Twister m_Twister;
    vector3 m_Origin, m_P1, m_P2, m_P3, m_P4, m_DX, m_DY;
    vector3 m_SR, m_CW;
    int m_CurID;
};

struct RenderData
{
    Renderer* engine;
    int start;
    int step;
};


#endif
