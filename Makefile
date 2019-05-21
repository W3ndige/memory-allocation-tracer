all: hooks load

hooks:
	gcc -shared -Wall -Wextra -ldl -fPIC src/lib/hooks.c -o hooks.so

load:
	gcc -Wall -Wextra src/loader/load.c -o load

clean:
	rm hooks.so load