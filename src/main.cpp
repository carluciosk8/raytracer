/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <cstdlib>

#include "Screen.h"
#include "Renderer.h"
#include "Scene.h"

#define SCRWIDTH    400
#define SCRHEIGHT   300


int main(int argc, char** argv)
{
    char defaultSceneFile[]="test1.in";
    char defaultImageFile[]="output.bmp";
    char* scene=0;
    char* image=0;
    unsigned int widht;
    unsigned int height;
    unsigned int procs;


    if (argc>1) scene  = argv[1];       else scene  = defaultSceneFile;
    if (argc>2) image  = argv[2];       else image  = defaultImageFile;
    if (argc>3) widht  = atoi(argv[3]); else widht  = SCRWIDTH;
    if (argc>4) height = atoi(argv[4]); else height = SCRHEIGHT;
    if (argc>5) procs  = atoi(argv[5]); else procs  = 1;


    Renderer* tracer   = new Renderer();
    Screen* opScreen = new Screen("Raytracer", widht, height);
    Scene*  opScene  = new Scene(scene);

    SDL_Thread* threads[procs];
    RenderData  data[procs];

    tracer->setScene(opScene);
    tracer->SetTarget(opScreen, widht, height);
    tracer->InitRender();

    for (int i = 0 ; i < procs ; ++i)
    {
        data[i] = { tracer, i, procs };
        threads[i] = SDL_CreateThread(Renderer::RenderP, &data[i]);
    }

    for (int i = 0 ; i < procs ; ++i)
    {
        SDL_WaitThread(threads[i], NULL);
    }

    opScreen->update();

    // wait key press
    SDL_Event event;
    bool keyPress = false;
    while(!keyPress)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYDOWN) keyPress = true;
    }

    opScreen->save(image);

    delete opScene;
    delete tracer;
    delete opScreen;

    return 0;
}
