CC = /usr/bin/clang++

build:
	$(CC) -std=c++20 -o main main.cpp \
src/physical_circle.cpp \
-I include \
-I/opt/homebrew/Cellar/sfml/2.6.1/include \
-L/opt/homebrew/Cellar/sfml/2.6.1/lib \
-lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm -f main	

run:
	./main
