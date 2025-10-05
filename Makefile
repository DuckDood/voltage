all: obj/ build/ obj/main.o obj/stb_image.o build/output 

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

build/output: obj/main.o obj/stb_image.o 
	${CXX} obj/main.o obj/stb_image.o  -o build/output -lGL -lGLEW -lglfw  

