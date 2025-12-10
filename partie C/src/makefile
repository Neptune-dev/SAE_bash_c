
all: cipher.a decipher.a findkey.a

cipher.a: cipher.o tools.o
	gcc $^ -o $@

decipher.a: decipher.o tools.o
	gcc $^ -o $@

findkey.a: findkey.o tools.o
	gcc $^ -o $@


cipher.o: cipher.c tools.h
	gcc -c $<

decipher.o: decipher.c tools.h
	gcc -c $<

findkey.o: findkey.c tools.h
	gcc -c $<

tools.o: tools.c
	gcc -c $<

clean:
	rm -f *.o cipher.a decipher.a findkey.a 