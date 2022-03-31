
CC = clang++
CFLAGS = -std=c++17 -O3

rubik3: rubik3.cc
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf rubik3