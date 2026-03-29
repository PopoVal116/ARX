CXX = g++
CXXFLAGS = -std=c++11 -Iimgui -Iimgui/backends -I/usr/include/SDL2  -I../../c_core/include #путь к заголовочным файлам
LIBS = -lSDL2 -lGLEW -lGL -lpthread

LDFLAGS = -L../../c_core/build -lcore #путь к директории с библиотекой

IMGUI_SOURCES = imgui/imgui.cpp \
                imgui/imgui_draw.cpp \
                imgui/imgui_tables.cpp \
                imgui/imgui_widgets.cpp \
                imgui/backends/imgui_impl_sdl2.cpp \
                imgui/backends/imgui_impl_opengl3.cpp

all: arh_gui

arh_gui: main.cpp $(IMGUI_SOURCES)
	$(CXX) $(CXXFLAGS) main.cpp $(IMGUI_SOURCES) -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -f arh_gui

run: arh_gui
	./arh_gui
