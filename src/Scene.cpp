/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Scene.h"

#include <iostream>
#include <fstream>
#include <string>

using std::ifstream;
using std::cout;
using std::endl;
using std::string;

#include "Command.h"
#include "Sphere.h"
#include "Pixmap.h"
#include "MarbleTexture.h"
#include "CellTexture.h"
#include "Material.h"


Scene::Scene()
{
}


Scene::Scene(const char* file)
{
    loadScene(file);
}


Scene::~Scene()
{
    for (unsigned int i=0; i<m_LightsList.size(); ++i) delete m_LightsList[i];
    for (unsigned int i=0; i<m_TexturesList.size(); ++i) delete m_TexturesList[i];
    for (unsigned int i=0; i<m_MaterialsList.size(); ++i) delete m_MaterialsList[i];
    for (unsigned int i=0; i<m_PrimitivesList.size(); ++i) delete m_PrimitivesList[i];
}



void Scene::loadScene(const char *filename)
{
    ifstream scene_file(filename);
    std::string line;

    while (std::getline(scene_file, line))
    {
        std::istringstream line_stream(line);
        std::string cmd;
        line_stream >> cmd;
        Command::parse(cmd, line_stream, this);
    }
}
