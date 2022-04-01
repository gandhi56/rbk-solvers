
CC = clang++
CFLAGS = -std=c++17 -O3
SRC = rubik3.cc

debug: $(SRC)
	mkdir -p build-debug
	$(CC) $(CFLAGS) $< -o build-debug/rubik3 -DDEBUG

release: $(SRC)
	mkdir -p build-release
	$(CC) $(CFLAGS) $< -o build-release/rubik3

clean:
	rm -rf build-debug build-release