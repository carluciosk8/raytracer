##***************************************************************************
##    Original code written in: Jun 2006    
##    copyright: (C) Carlucio Santos Cordeiro
##***************************************************************************
sources = [
'src/Box.cpp',
'src/CellTexture.cpp',
'src/Command.cpp',
'src/Material.cpp',
'src/MarbleTexture.cpp',
'src/Pixmap.cpp',
'src/Plane.cpp',
'src/Primitive.cpp',
'src/Renderer.cpp',
'src/Scene.cpp',
'src/Screen.cpp',
'src/Sphere.cpp',
'src/spline.cpp',
'src/Texture.cpp',
'src/Twister.cpp',
'src/main.cpp'
]

libs=['SDL']

Program('raytracer', sources, LIBS=libs, CXX='g++ -std=c++0x', CCFLAGS='`sdl-config --cflags`')
