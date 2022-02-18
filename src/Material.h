/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Command.h"

class Material
{
public:
    Material();
    Material( double amb, double diff, double spec, double shin, double refl, double refr, double ridx ) :
      m_Amb(amb), m_Diff(diff), m_Spec(spec), m_Shin(shin), m_Refl(refl), m_Refr(refr), m_RIndex(ridx) {}
    void SetAmbient( double amb ) { m_Amb = amb; }
    void SetDiffuse( double diff ) { m_Diff = diff; }
    void SetSpecular( double spec ) { m_Spec = spec; }
    void SetShininess( double shin ) { m_Shin = shin; }
    void SetReflection( double refl ) { m_Refl = refl; }
    void SetRefraction( double refr ) { m_Refr = refr; }
    void SetRefrIndex( double ridx ) { m_RIndex = ridx; }

    double GetAmbient() { return m_Amb; }
    double GetSpecular() { return m_Spec; }
    double GetDiffuse() { return m_Diff; }
    double GetShininess() { return m_Shin; }
    double GetReflection() { return m_Refl; }
    double GetRefraction() { return m_Refr; }
    double GetRefrIndex() { return m_RIndex; }

private:
    double m_Amb, m_Diff, m_Spec, m_Shin;
    double m_Refl, m_Refr;
    double m_RIndex;
};


DECLARE_COMMAND(Material);


#endif
