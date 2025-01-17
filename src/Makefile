game: game.o ia.o
	gcc -o game game.o ia.o
	rm *.o


%.o: %.c
	gcc -g -c -c $<

debug: game.o ia.o
	gcc -fsanitize=address -o game game.o ia.o

clean:
	rm *.o game

