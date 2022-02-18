##***************************************************************************
##    Original code written in: Jun 2006    
##    copyright: (C) Carlucio Santos Cordeiro
##***************************************************************************
CC = g++
CFLAGS = -g -Wall $$(sdl-config --cflags) -c
LDFLAGS = $$(sdl-config --libs)
SOURCES = \
    src/Box.cpp \
    src/Engine.cpp \
    src/MarbleTexture.cpp \
    src/Pixmap.cpp \
    src/Plane.cpp \
    src/Scene.cpp \
    src/Screen.cpp \
    src/Sphere.cpp \
    src/Texture.cpp \
    src/Twister.cpp \
    src/main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = raytracer

all: $(SOURCES) $(EXECUTABLE)
	

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm src/*.o $(EXECUTABLE)
