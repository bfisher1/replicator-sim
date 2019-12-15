CC = g++
CFLAGS = -std=c++11 -g 

Rep : main.o graphics.o anim.o world.o perlin.o util.o bot.o
	$(CC) $(CFLAGS) -o $@ $^ -lX11 -lm -I/usr/local/include -L/usr/local/lib -lSDL

world.o : world.cpp world.hpp graphics.h perlin.h util.h bot.hpp

graphics.o : graphics.c graphics.h util.h

anim.o : anim.c anim.h graphics.h util.h

perlin.o : perlin.c perlin.h util.h

main.o : main.cpp graphics.h anim.h world.hpp util.h

util.o : util.c util.h

bot.o : bot.cpp bot.hpp world.hpp graphics.h

clean:
	rm -f main.o graphics.o anim.o world.o perlin.o util.o bot.o
