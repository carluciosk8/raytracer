/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Material.h"
#include "Scene.h"


Material::Material() :
    m_Refl( 0 ), m_Diff( 0.2f ), m_Spec( 0.8f ),
    m_RIndex( 1.5f )//, m_DRefl( 0 ), m_Texture( 0 )
{
    //Command::register_command("material", new CmdMaterial(this));
}


void CmdMaterial::parse(std::istringstream& line, Scene* scene)
{
    Material* opMaterial;
    double amb, diff, spec, shin, refl, refr, refrIdx;

    read_number(line, amb);
    read_number(line, diff);
    read_number(line, spec);
    read_number(line, shin);
    read_number(line, refl);
    read_number(line, refr);
    read_number(line, refrIdx);

    opMaterial = new Material(amb , diff , spec , shin , refl , refr , refrIdx);
    scene->addMaterial(opMaterial);

    //std::cout << "opMaterial: " << amb << " , " << diff << " , "  << spec << " , "  << shin << " , " << refl << " , " << refr << " , " << refrIdx << std::endl;
}
