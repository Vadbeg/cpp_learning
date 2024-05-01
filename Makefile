CC = /usr/bin/clang++

build:
	$(CC) -std=c++20 -o main main.cpp \
-I include \
-I/opt/homebrew/Cellar/sdl2/2.30.2/include \
-L/opt/homebrew/Cellar/sdl2/2.30.2/lib \
-lSDL2

clean:
	rm -f main	

run:
	./main
