all: obj/ build/ obj/main.o obj/stb_image.o build/quasar static/imgui/objCache.o build/cacheObj 

clean:
	rm -r obj/ build/ 
.PHONY: clean

obj/: 
	mkdir -p obj/

build/: 
	mkdir -p build/

obj/main.o: src/main.cpp
	${CXX} src/main.cpp -c -o obj/main.o  -Iinclude/ 

obj/stb_image.o: src/stb_image.cpp
	${CXX} src/stb_image.cpp -c -o obj/stb_image.o  

build/quasar: obj/main.o obj/stb_image.o static/imgui/imgui_demo.o static/imgui/imgui_draw.o static/imgui/imgui_impl_glfw.o static/imgui/imgui_impl_opengl3.o static/imgui/imgui.o static/imgui/imgui_tables.o static/imgui/imgui_widgets.o 
	${CXX} obj/main.o obj/stb_image.o static/imgui/imgui_demo.o static/imgui/imgui_draw.o static/imgui/imgui_impl_glfw.o static/imgui/imgui_impl_opengl3.o static/imgui/imgui.o static/imgui/imgui_tables.o static/imgui/imgui_widgets.o  -o build/quasar -lGL -lGLEW -lglfw  

static/imgui/objCache.o: src/objCache.cpp
	${CXX} src/objCache.cpp -c -o static/imgui/objCache.o  

build/cacheObj: static/imgui/objCache.o 
	${CXX} static/imgui/objCache.o  -o build/cacheObj  

