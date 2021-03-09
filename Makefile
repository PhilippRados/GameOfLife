game:
	 cc main.c -o play -I include -L lib -l SDL2-2.0.0 

debug:
	 cc -g main.c -o db -I include -L lib -l SDL2-2.0.0 

test_main:
	 cc test.c -o test -I include -L lib -l criterion.3.1.0
