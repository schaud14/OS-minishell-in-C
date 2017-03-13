all:prog
prog:
	gcc -g -o minish minish.c

run:
	./minish

clean:
	rm -f minish *~