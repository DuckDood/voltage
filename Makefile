all: obj/ static/imgui/obj/ static/stb_image/obj/ build/ static/imgui/obj/imgui_demo.o static/imgui/obj/imgui_draw.o static/imgui/obj/imgui_impl_sdl3.o static/imgui/obj/imgui_impl_opengl3.o static/imgui/obj/imgui.o static/imgui/obj/imgui_tables.o static/imgui/obj/imgui_widgets.o static/stb_image/obj/stb_image.o obj/voltage.o obj/main.o build/voltage obj/objCache.o build/cacheObj 

clean:
	rm -r obj/ build/ 
.PHONY: clean

obj/: 
	mkdir -p obj/

static/imgui/obj/: 
	mkdir -p static/imgui/obj/

static/stb_image/obj/: 
	mkdir -p static/stb_image/obj/

build/: 
	mkdir -p build/

cleanstatic:
	rm -r static/imgui/obj/ static/stb_image/obj/
.PHONY: cleanstatic
static/imgui/obj/imgui_demo.o: static/imgui/src/imgui_demo.cpp
	${CXX} static/imgui/src/imgui_demo.cpp -c -o static/imgui/obj/imgui_demo.o  

static/imgui/obj/imgui_draw.o: static/imgui/src/imgui_draw.cpp
	${CXX} static/imgui/src/imgui_draw.cpp -c -o static/imgui/obj/imgui_draw.o  

static/imgui/obj/imgui_impl_sdl3.o: static/imgui/src/imgui_impl_sdl3.cpp
	${CXX} static/imgui/src/imgui_impl_sdl3.cpp -c -o static/imgui/obj/imgui_impl_sdl3.o  

static/imgui/obj/imgui_impl_opengl3.o: static/imgui/src/imgui_impl_opengl3.cpp
	${CXX} static/imgui/src/imgui_impl_opengl3.cpp -c -o static/imgui/obj/imgui_impl_opengl3.o  

static/imgui/obj/imgui.o: static/imgui/src/imgui.cpp
	${CXX} static/imgui/src/imgui.cpp -c -o static/imgui/obj/imgui.o  

static/imgui/obj/imgui_tables.o: static/imgui/src/imgui_tables.cpp
	${CXX} static/imgui/src/imgui_tables.cpp -c -o static/imgui/obj/imgui_tables.o  

static/imgui/obj/imgui_widgets.o: static/imgui/src/imgui_widgets.cpp
	${CXX} static/imgui/src/imgui_widgets.cpp -c -o static/imgui/obj/imgui_widgets.o  

static/stb_image/obj/stb_image.o: static/stb_image/src/stb_image.c
	${CC} static/stb_image/src/stb_image.c -c -o static/stb_image/obj/stb_image.o  -Iinclude/ 

obj/voltage.o: src/voltage.cpp src/defines.h
	${CXX} src/voltage.cpp -c -o obj/voltage.o  -Iinclude/ 

obj/main.o: src/main.cpp src/defines.h
	${CXX} src/main.cpp -c -o obj/main.o  -Iinclude/ 

build/voltage: static/imgui/obj/imgui_demo.o static/imgui/obj/imgui_draw.o static/imgui/obj/imgui_impl_sdl3.o static/imgui/obj/imgui_impl_opengl3.o static/imgui/obj/imgui.o static/imgui/obj/imgui_tables.o static/imgui/obj/imgui_widgets.o static/stb_image/obj/stb_image.o obj/voltage.o obj/main.o 
	${CXX} static/imgui/obj/imgui_demo.o static/imgui/obj/imgui_draw.o static/imgui/obj/imgui_impl_sdl3.o static/imgui/obj/imgui_impl_opengl3.o static/imgui/obj/imgui.o static/imgui/obj/imgui_tables.o static/imgui/obj/imgui_widgets.o static/stb_image/obj/stb_image.o obj/voltage.o obj/main.o  -o build/voltage -lGL -lGLEW -lSDL3  

obj/objCache.o: src/objCache.cpp
	${CXX} src/objCache.cpp -c -o obj/objCache.o  

build/cacheObj: obj/objCache.o 
	${CXX} obj/objCache.o  -o build/cacheObj  

