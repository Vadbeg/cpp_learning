CC = /usr/bin/clang++

build:
	$(CC) -std=c++20 -o main main.cpp

clean:
	rm -f main	

run:
	./main
