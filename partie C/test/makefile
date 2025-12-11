
all: test

test: test.o tools.o
	gcc $^ -o $@


test.o: test.c tools.h
	gcc -c $<

tools.o: tools.c
	gcc -c $<

clean:
	rm -f *.o test