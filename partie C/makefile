INCLUDE = ./include
SRC = ./src
CFLAGS = -I$(INCLUDE)

# regle defaut
all: cipher decipher findkey saelib.a

# executables
cipher: cipher.o tools.o
	gcc $^ -o $@

decipher: decipher.o decipherbody.o tools.o
	gcc $^ -o $@

findkey: findkey.o findkeybody.o tools.o
	gcc $^ -o $@

# objets
cipher.o: $(SRC)/cipher.c $(INCLUDE)/tools.h
	gcc $(CFLAGS) -c $<

decipher.o: $(SRC)/decipher.c $(INCLUDE)/decipherbody.h
	gcc $(CFLAGS) -c $<

decipherbody.o: $(SRC)/decipherbody.c $(INCLUDE)/tools.h
	gcc $(CFLAGS) -c $<

findkey.o: $(SRC)/findkey.c $(INCLUDE)/findkeybody.h
	gcc $(CFLAGS) -c $<

findkeybody.o: $(SRC)/findkeybody.c $(INCLUDE)/tools.h
	gcc $(CFLAGS) -c $<

tools.o: $(SRC)/tools.c $(INCLUDE)/tools.h
	gcc $(CFLAGS) -c $<

# lib
saelib.a: decipherbody.o findkeybody.o
	ar rcs $@ $^
	ranlib saelib.a

# clean
.PHONY: clean

clean:
	rm -f *.o cipher decipher findkey saelib.a