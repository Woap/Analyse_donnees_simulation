CC=gcc
CFLAGS=-c -g -Wall

all : bin/analyse

bin/analyse: obj/analyse.o obj/flux.o obj/global.o obj/paquet.o obj/noeud.o
	$(CC) -o analyse obj/analyse.o obj/flux.o obj/global.o obj/paquet.o obj/noeud.o -lm
	mv analyse bin/

obj/flux.o : src/flux.c include/flux.h
	$(CC) $(CFLAGS) src/flux.c include/flux.h -O2
	mv flux.o obj/

obj/global.o : src/global.c include/global.h
	$(CC) $(CFLAGS) src/global.c include/global.h -O2
	mv global.o obj/

obj/paquet.o : src/paquet.c include/paquet.h
	$(CC) $(CFLAGS) src/paquet.c include/paquet.h -O2
	mv paquet.o obj/

obj/noeud.o : src/noeud.c include/noeud.h
	$(CC) $(CFLAGS) src/noeud.c include/noeud.h -O2
	mv noeud.o obj/

obj/analyse.o : src/analyse.c include/flux.h include/global.h include/paquet.h include/noeud.h
	$(CC) $(CFLAGS) src/analyse.c include/flux.h include/global.h include/paquet.h include/noeud.h -O2
	mv analyse.o obj/

archive :
	tar -czvf archive_ibis.tar.gz ./

clean :
	rm -rf obj/* bin/analyse
