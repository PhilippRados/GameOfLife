test_main:
	cc -c shared.c -I include -o main.o
	cc -c test.c -I include -o test.o
	cc -o testing main.o test.o -L lib -lcriterion.3.1.0 -lSDL2-2.0.0 

shared_game:
	cc -c shared.c -I include -o shared.o
	cc -c main.c -I include -o main.o
	cc -o game_of_life main.o shared.o -L lib -lSDL2-2.0.0

clean:
	rm -f *.o
