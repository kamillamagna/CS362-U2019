CFLAGS= -Wall -fpic -coverage -lm -std=c99
testme: testme.c
	gcc -o testme testme.c -g $(CFLAGS)
	./testme > /dev/null
	gcov -b testme.c > testme.out
	cat testme.c.gcov >> testme.out
