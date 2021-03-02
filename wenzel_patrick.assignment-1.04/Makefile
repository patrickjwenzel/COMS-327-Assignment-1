build:
	gcc -Wall dungeon.c -lm -o dungeon.o
	
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

clean:
	rm *.o
