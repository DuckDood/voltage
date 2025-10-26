all: obj/ build/ obj/main.o obj/voltage.o build/voltage obj/objCache.o build/cacheObj 

clean:
	rm -r obj/ build/ 
.PHONY: clean

obj/: 
	mkdir -p obj/

build/: 
	mkdir -p build/

obj/main.o: src/main.cpp src/defines.h
	${CXX} src/main.cpp -c -o obj/main.o  -Iinclude/ 

obj/voltage.o: src/voltage.cpp src/defines.h
	${CXX} src/voltage.cpp -c -o obj/voltage.o  -Iinclude/ 

build/voltage: obj/main.o obj/voltage.o static/imgui/imgui_demo.o static/imgui/imgui_draw.o static/imgui/imgui_impl_sdl3.o static/imgui/imgui_impl_opengl3.o static/imgui/imgui.o static/imgui/imgui_tables.o static/imgui/imgui_widgets.o static/stb_image.o 
	${CXX} obj/main.o obj/voltage.o static/imgui/imgui_demo.o static/imgui/imgui_draw.o static/imgui/imgui_impl_sdl3.o static/imgui/imgui_impl_opengl3.o static/imgui/imgui.o static/imgui/imgui_tables.o static/imgui/imgui_widgets.o static/stb_image.o  -o build/voltage -lGL -lGLEW -lSDL3  

obj/objCache.o: src/objCache.cpp
	${CXX} src/objCache.cpp -c -o obj/objCache.o  

build/cacheObj: obj/objCache.o 
	${CXX} obj/objCache.o  -o build/cacheObj  

