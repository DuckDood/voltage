all: obj/ build/ static/imgui/obj/ static/stb_image/obj/ static/imgui/obj/imgui_demo.cpp.o static/imgui/obj/imgui_draw.cpp.o static/imgui/obj/imgui_impl_sdl3.cpp.o static/imgui/obj/imgui_impl_opengl3.cpp.o static/imgui/obj/imgui.cpp.o static/imgui/obj/imgui_tables.cpp.o static/imgui/obj/imgui_widgets.cpp.o static/stb_image/obj/stb_image.c.o obj/voltage.cpp.o obj/main.cpp.o build/voltage obj/objCache.cpp.o build/cacheObj 
OSMODE := l

obj/:
ifeq (${OSMODE}, l)
	mkdir -p obj
else
	mkdir obj
endif

build/:
ifeq (${OSMODE}, l)
	mkdir -p build
else
	mkdir build
endif

static/imgui/obj/:
ifeq (${OSMODE}, l)
	mkdir -p static/imgui/obj
else
	mkdir static/imgui/obj
endif

static/stb_image/obj/:
ifeq (${OSMODE}, l)
	mkdir -p static/stb_image/obj
else
	mkdir static/stb_image/obj
endif

clean:
	rm -r obj/
	rm -r build/
.PHONY: clean
cleanstatic:
	rm -r static/imgui/obj/ static/stb_image/obj/
.PHONY: cleanstatic

static/imgui/obj/imgui_demo.cpp.o: static/imgui/src/imgui_demo.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui_demo.cpp -c -o static/imgui/obj/imgui_demo.cpp.o
else
	${CXX} static/imgui/src/imgui_demo.cpp -c -o static/imgui/obj/imgui_demo.cpp.o
endif


static/imgui/obj/imgui_draw.cpp.o: static/imgui/src/imgui_draw.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui_draw.cpp -c -o static/imgui/obj/imgui_draw.cpp.o
else
	${CXX} static/imgui/src/imgui_draw.cpp -c -o static/imgui/obj/imgui_draw.cpp.o
endif


static/imgui/obj/imgui_impl_sdl3.cpp.o: static/imgui/src/imgui_impl_sdl3.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui_impl_sdl3.cpp -c -o static/imgui/obj/imgui_impl_sdl3.cpp.o
else
	${CXX} static/imgui/src/imgui_impl_sdl3.cpp -c -o static/imgui/obj/imgui_impl_sdl3.cpp.o
endif


static/imgui/obj/imgui_impl_opengl3.cpp.o: static/imgui/src/imgui_impl_opengl3.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui_impl_opengl3.cpp -c -o static/imgui/obj/imgui_impl_opengl3.cpp.o
else
	${CXX} static/imgui/src/imgui_impl_opengl3.cpp -c -o static/imgui/obj/imgui_impl_opengl3.cpp.o
endif


static/imgui/obj/imgui.cpp.o: static/imgui/src/imgui.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui.cpp -c -o static/imgui/obj/imgui.cpp.o
else
	${CXX} static/imgui/src/imgui.cpp -c -o static/imgui/obj/imgui.cpp.o
endif


static/imgui/obj/imgui_tables.cpp.o: static/imgui/src/imgui_tables.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui_tables.cpp -c -o static/imgui/obj/imgui_tables.cpp.o
else
	${CXX} static/imgui/src/imgui_tables.cpp -c -o static/imgui/obj/imgui_tables.cpp.o
endif


static/imgui/obj/imgui_widgets.cpp.o: static/imgui/src/imgui_widgets.cpp
ifeq (${OSMODE}, l)
	${CXX} static/imgui/src/imgui_widgets.cpp -c -o static/imgui/obj/imgui_widgets.cpp.o
else
	${CXX} static/imgui/src/imgui_widgets.cpp -c -o static/imgui/obj/imgui_widgets.cpp.o
endif


static/stb_image/obj/stb_image.c.o: static/stb_image/src/stb_image.c
ifeq (${OSMODE}, l)
	${CXX} static/stb_image/src/stb_image.c -c -o static/stb_image/obj/stb_image.c.o -Iinclude/
else
	${CXX} static/stb_image/src/stb_image.c -c -o static/stb_image/obj/stb_image.c.o -Iinclude/
endif


obj/voltage.cpp.o: src/voltage.cpp
ifeq (${OSMODE}, l)
	${CXX} src/voltage.cpp -c -o obj/voltage.cpp.o -Iinclude/
else
	${CXX} src/voltage.cpp -c -o obj/voltage.cpp.o -Iinclude/
endif


obj/main.cpp.o: src/main.cpp
ifeq (${OSMODE}, l)
	${CXX} src/main.cpp -c -o obj/main.cpp.o -Iinclude/
else
	${CXX} src/main.cpp -c -o obj/main.cpp.o -Iinclude/
endif


build/voltage: static/imgui/obj/imgui_demo.cpp.o static/imgui/obj/imgui_draw.cpp.o static/imgui/obj/imgui_impl_sdl3.cpp.o static/imgui/obj/imgui_impl_opengl3.cpp.o static/imgui/obj/imgui.cpp.o static/imgui/obj/imgui_tables.cpp.o static/imgui/obj/imgui_widgets.cpp.o static/stb_image/obj/stb_image.c.o obj/voltage.cpp.o obj/main.cpp.o 
ifeq (${OSMODE}, l)
	${CXX} static/imgui/obj/imgui_demo.cpp.o static/imgui/obj/imgui_draw.cpp.o static/imgui/obj/imgui_impl_sdl3.cpp.o static/imgui/obj/imgui_impl_opengl3.cpp.o static/imgui/obj/imgui.cpp.o static/imgui/obj/imgui_tables.cpp.o static/imgui/obj/imgui_widgets.cpp.o static/stb_image/obj/stb_image.c.o obj/voltage.cpp.o obj/main.cpp.o -o build/voltage -lGL -lGLEW -lSDL3
else
	${CXX} static/imgui/obj/imgui_demo.cpp.o static/imgui/obj/imgui_draw.cpp.o static/imgui/obj/imgui_impl_sdl3.cpp.o static/imgui/obj/imgui_impl_opengl3.cpp.o static/imgui/obj/imgui.cpp.o static/imgui/obj/imgui_tables.cpp.o static/imgui/obj/imgui_widgets.cpp.o static/stb_image/obj/stb_image.c.o obj/voltage.cpp.o obj/main.cpp.o -o build/voltage -lopengl32 -lglew32 -lSDL3
endif


obj/objCache.cpp.o: src/objCache.cpp
ifeq (${OSMODE}, l)
	${CXX} src/objCache.cpp -c -o obj/objCache.cpp.o -Iinclude/
else
	${CXX} src/objCache.cpp -c -o obj/objCache.cpp.o -Iinclude/
endif


build/cacheObj: obj/objCache.cpp.o 
ifeq (${OSMODE}, l)
	${CXX} obj/objCache.cpp.o -o build/cacheObj -lGL -lGLEW -lSDL3
else
	${CXX} obj/objCache.cpp.o -o build/cacheObj -lopengl32 -lglew32 -lSDL3
endif


