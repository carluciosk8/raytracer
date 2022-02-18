/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Screen.h"

#include <SDL/SDL.h>

#include <iostream>

using std::cout;
using std::endl;



Screen::Screen(const char* cpName, int iWidth, int iHeight)
: m_iWidth(iWidth),
  m_iHeight(iHeight)
{
    int  iFlags = SDL_ANYFORMAT;


    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        cout << "Couldn't initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_WM_SetCaption(cpName,0);

    screen_mutex = SDL_CreateMutex();

    const SDL_VideoInfo* video = SDL_GetVideoInfo();

    if( !video )
    {
        cout << "Couldn't get video information: " << SDL_GetError() << endl;
        exit(1);
    }

    if( !(m_opScreen = SDL_SetVideoMode( iWidth, iHeight, video->vfmt->BitsPerPixel, iFlags )) )
    {
        cout << "Couldn't set video mode: " << SDL_GetError() << endl;
        exit(1);
    }
}



Screen::~Screen()
{
    printf("Quitting SDL\n");
    SDL_DestroyMutex(screen_mutex);
    SDL_Quit();
}



void Screen::beginRender()
{
    SDL_LockMutex(screen_mutex);
    SDL_LockSurface(m_opScreen);
}



void Screen::endRender()
{
    //SDL_UnlockSurface(m_opScreen);
    SDL_UpdateRect(m_opScreen, 0, 0, 0, 0);
    //SDL_UnlockMutex(screen_mutex);
}



void Screen::putpixel(int x, int y, Uint32 pixel)
{
    //beginRender();

    int bpp = m_opScreen->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)m_opScreen->pixels + y * m_opScreen->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }

    //endRender();
}
