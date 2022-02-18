/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __SCREEN_H__
#define __SCREEN_H__


#include <SDL/SDL.h>


class Screen
{
public:
    Screen(const char* cpName, int iWidth, int iHeight);
   ~Screen();

    inline int getWidth() { return m_iWidth; }
    inline int getHeight() { return m_iHeight; }
    inline SDL_Surface* getSDLSurface() { return m_opScreen; }
    void save(const char* file) { SDL_SaveBMP(m_opScreen, file); }

    void putpixel(int x, int y, Uint32 pixel);

    void beginRender();
    void endRender();
    void update() { SDL_LockMutex(screen_mutex); SDL_UpdateRect(m_opScreen, 0, 0, 0, 0); SDL_UnlockMutex(screen_mutex); }

protected:
    SDL_Surface*  m_opScreen;
    SDL_mutex* screen_mutex;

    int  m_iWidth;
    int  m_iHeight;
};


#endif
