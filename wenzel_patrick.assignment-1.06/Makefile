NUMMON =

biuld:
	g++ -Wall dungeon.cpp -lm -lncurses -o dungeon.o

build:
	g++ -Wall dungeon.cpp -lm -lncurses -o dungeon.o
	
run: dungeon.o
	./dungeon.o

load: dungeon.o
	./dungeon.o --load

save: dungeon.o
	./dungeon.o --save

sl: dungeon.o
	./dungeon.o --save --load
	
ls: dungeon.o
	./dungeon.o --load --save
	
nummon: dungeon.o
	./dungeon.o --nummon $(NUMMON)

lsn:	dungeon.o
	./dungeon.o --load --save --nummon $(NUMMON)

lns:	dungeon.o
	./dungeon.o --load --nummon $(NUMMON) --save

snl:	dungeon.o
	./dungeon.o --save --nummon $(NUMMON) --load

sln:	dungeon.o
	./dungeon.o --save --load --nummon $(NUMMON)
	
nsl:	dungeon.o
	./dungeon.o --nummon $(NUMMON) --save --load

nls:	dungeon.o
	./dungeon.o --nummon $(NUMMON) --load --save

clean:
	rm *.o

debug: dungeon.o
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=debug.txt ./dungeon.o

debug_nummon: dungeon.o
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=debug.txt ./dungeon.o --nummon $(NUMMON)
