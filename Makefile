all: rubik3
rubik3: rubik3.cc
	clang++ rubik3.cc -o rubik3

clean:
	rm -rf rubik3