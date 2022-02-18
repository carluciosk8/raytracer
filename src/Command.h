/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

class Scene;
class vector3;


class Command
{
public:
    //Command();
    virtual void parse(std::istringstream&, Scene*) = 0;

    static void register_command(const std::string& name, Command* cmd) { mCommandTable[name] = cmd; }
    static void parse(const std::string&, std::istringstream&, Scene*);

private:
    static std::unordered_map<std::string, Command*>  mCommandTable;
};



#define DECLARE_COMMAND(CMD) \
    class Cmd##CMD : public Command  \
    {                           \
    public:                     \
        Cmd##CMD() {}\
        virtual void parse(std::istringstream&, Scene*); \
    }

#define REGISTER_COMMAND(CMD) \
{ \
    static bool __first_run__ = true; \
    if (__first_run__) \
    { \
        std::cout << "recording command: " << #CMD << std::endl; \
        Command::register_command(#CMD, new Cmd##CMD()); \
        __first_run__ = false; \
    } \
}




template <typename NUMBER>
void read_number(std::istringstream& iss, NUMBER &value)
{
    std::string str;
    iss >> str;

    if (str == "rnd")
    {
        NUMBER a, b;
        iss >> a >> b;
        value = (b - a)*(static_cast<double>(rand())/RAND_MAX) + a;
    }
    else
    {
        double foo = std::stod(str);
        value = foo;
    }
}

void read_vector3(std::istringstream &iss, vector3& vec);


#endif // __COMMAND_H__
