CC = /usr/bin/clang++

build:
	$(CC) -std=c++20 -o main main.cpp \
src/physical_circle.cpp \
-I include \
-I/opt/homebrew/Cellar/sfml/2.6.1/include \
-L/opt/homebrew/Cellar/sfml/2.6.1/lib \
-I/opt/homebrew/Cellar/sdl2/2.30.2/include \
-L/opt/homebrew/Cellar/sdl2/2.30.2/lib \
-lsfml-graphics -lsfml-window -lsfml-system \
-lSDL2
clean:
	rm -f main	

run:
	./main
