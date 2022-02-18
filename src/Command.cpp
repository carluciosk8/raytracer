/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Command.h"

#include <string>
#include <iostream>

#include "MathLib.h"
#include "Scene.h"

#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

#include "MarbleTexture.h"
#include "CellTexture.h"
#include "Pixmap.h"


std::unordered_map<std::string, Command*> Command::mCommandTable;

//Command::Command()
//{
//}

void Command::parse(const std::string& cmd, std::istringstream& line, Scene* scene)
{
    if (mCommandTable[cmd])
    {
        mCommandTable[cmd]->parse(line, scene);
    }
    else
    {

        std::cout << "invalid command: " << cmd << std::endl;
    }
}


void CmdComment::parse(std::istringstream& iss, Scene* scene)
{
    // do nothing
}


void CmdSeed::parse(std::istringstream& iss, Scene* scene)
{
    std::string seed;
    iss >> seed;
    static int counter = 1;
    if (seed == "time") srand(counter++^time(0)); else srand(std::stoi(seed));
}


void CmdCamera::parse(std::istringstream& iss, Scene* scene)
{
    vector3 position;
    vector3 lookat;
    vector3 up;
    double  fov;

    read_vector3(iss, position);
    read_vector3(iss, lookat);
    read_vector3(iss, up);
    read_number(iss, fov);

    scene->getCamera().setEye(position);
    scene->getCamera().setLook(lookat);
    scene->getCamera().setUp(up);
    scene->getCamera().setFoV(fov);
}


void CmdAmbient::parse(std::istringstream& iss, Scene* scene)
{
    vector3 vLightColor;
    read_vector3(iss, vLightColor);
    scene->setAmbient(vLightColor);
}


void read_vector3(std::istringstream &iss, vector3& vec)
{
    read_number(iss, vec.x);
    read_number(iss, vec.y);
    read_number(iss, vec.z);
}
