
CC = clang++
CFLAGS = -std=c++17
SRC = rubik3.cc

debug: $(SRC)
	mkdir -p build-debug
	$(CC) $(CFLAGS) -g -O0 $< -o build-debug/rubik3 -DDEBUG

release: $(SRC)
	mkdir -p build-release
	$(CC) $(CFLAGS) -O3 $< -o build-release/rubik3

clean:
	rm -rf build-debug build-release