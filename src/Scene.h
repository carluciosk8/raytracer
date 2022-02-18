/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __SCENE_H__
#define __SCENE_H__


#include <vector>
#include <unordered_map>
#include <sstream>

using std::vector;

#include "Primitive.h"

class Command;

class Comment{};
class Seed{};
class Ambient{};

class Camera
{
public:
    inline vector3& getEye()            { return m_vEye; }
    inline vector3& getLook()           { return m_vLook; }
    inline vector3& getUp()             { return m_vUp; }
    inline double   getFoV()            { return m_fFoV; }

    inline vector3& setEye(const vector3& v)      { m_vEye = v; }
    inline vector3& setLook(const vector3& v)     { m_vLook = v; }
    inline vector3& setUp(const vector3& v)       { m_vUp = v; }
    inline double   setFoV(double s)              { m_fFoV = s; }

protected:
    vector3  m_vEye;
    vector3  m_vLook;
    vector3  m_vUp;
    double   m_fFoV;
};


class Scene
{
public:
    Scene();
    Scene(const char* file);
   ~Scene();

    void loadScene(const char* cpFile);

    inline Camera& getCamera() { return mCamera; }

    inline void   setAmbient(const vector3& v)  { m_vAmbient = v;    }
    inline Color& getAmbient()                  { return m_vAmbient; }

    inline void addPrimitive (Primitive* primitive) { m_PrimitivesList.push_back(primitive); }
    inline void addLight     (Primitive* light)     { m_LightsList.push_back(light);         }
    inline void addMaterial  (Material* material)   { m_MaterialsList.push_back(material);   }
    inline void addTexture   (Texture* texture)     { m_TexturesList.push_back(texture);     }

    inline Primitive* getPrimitive (int i) { return m_PrimitivesList[i]; }
    inline Primitive* getLight     (int i) { return m_LightsList[i];     }
    inline Material*  getMaterial  (int i) { return m_MaterialsList[i];  }
    inline Texture*   getTexture   (int i) { return m_TexturesList[i];   }

    inline unsigned int  GetNrPrimitives()   { return static_cast<unsigned int>(m_PrimitivesList.size()); }
    inline unsigned int  GetNrLights()       { return static_cast<unsigned int>(m_LightsList.size()); }

    inline Primitive*    GetPrimitive(int i) { return m_PrimitivesList[i]; }
    inline Primitive*    GetLight(int i)     { return m_LightsList[i]; }


protected:
    Camera mCamera;
    Color  m_vAmbient;

    vector<Primitive*>  m_PrimitivesList;
    vector<Primitive*>  m_LightsList;
    vector<Material*>   m_MaterialsList;
    vector<Texture*>    m_TexturesList;

    std::unordered_map<std::string, Command*>  mCommandTable;
};

DECLARE_COMMAND(Comment);
DECLARE_COMMAND(Seed);
DECLARE_COMMAND(Camera);
DECLARE_COMMAND(Ambient);


#endif
