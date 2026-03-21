CXX = g++
CXXFLAGS = -std=c++11 -Iimgui -Iimgui/backends -I/usr/include/SDL2
LIBS = -lSDL2 -lGLEW -lGL -lpthread

IMGUI_SOURCES = imgui/imgui.cpp \
                imgui/imgui_draw.cpp \
                imgui/imgui_tables.cpp \
                imgui/imgui_widgets.cpp \
                imgui/backends/imgui_impl_sdl2.cpp \
                imgui/backends/imgui_impl_opengl3.cpp

all: arh_gui

arh_gui: main.cpp $(IMGUI_SOURCES)
	$(CXX) $(CXXFLAGS) main.cpp $(IMGUI_SOURCES) -o $@ $(LIBS)

clean:
	rm -f arh_gui

run: arh_gui
	./arh_gui


# Находясь в папке arh: wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h