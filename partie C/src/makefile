
all: cipher decipher findkey

cipher: cipher.o tools.o
	gcc $^ -o $@

decipher: decipher.o tools.o
	gcc $^ -o $@

findkey: findkey.o tools.o
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
	rm -f *.o cipher decipher findkey 