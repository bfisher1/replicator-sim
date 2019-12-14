CC = g++
CFLAGS = -std=c++11 -g 

Rep : main.o graphics.o anim.o world.o perlin.o
	$(CC) $(CFLAGS) -o $@ $^ -lX11 -lm -I/usr/local/include -L/usr/local/lib -lSDL

world.o : world.cpp world.hpp graphics.h perlin.h

graphics.o : graphics.c graphics.h

anim.o : anim.c anim.h graphics.h

perlin.o : perlin.c perlin.h

main.o : main.cpp graphics.h anim.h world.hpp

clean:
	rm -f main.o graphics.o anim.o world.o perlin.o
