OUT = ../
INCLUDE = ./include
SRC = ./
CFLAGS = -I$(INCLUDE)

# regle defaut
all: $(OUT)cipher $(OUT)decipher $(OUT)findkey $(OUT)libsae.a

# executables
$(OUT)cipher: cipher.o tools.o
	gcc $^ -o $@

$(OUT)decipher: decipher.o decipherbody.o tools.o
	gcc $^ -o $@

$(OUT)findkey: findkey.o findkeybody.o tools.o
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
$(OUT)libsae.a: decipherbody.o findkeybody.o
	ar rcs $@ $^
	ranlib $@

# clean
.PHONY: clean

clean:
	rm -f *.o $(OUT)cipher $(OUT)decipher $(OUT)findkey $(OUT)libsae.a