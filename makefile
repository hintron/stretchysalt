BIN = mkdir client

all : $(BIN)

mkdir :
	mkdir -p bin

client : mkdir
	g++ -g main.cpp Stretchysalt.cpp -std=c++11 -lcrypto -o bin/test.out

clean :
	rm -rf bin/

.PHONY: All, Clean
