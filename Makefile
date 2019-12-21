CC = g++
CFLAGS = -Wall -std=c++11 -g

Rep : main.o graphics.o anim.o world.o perlin.o util.o bot.o resourceGraph.o loc.o navigate.o innerWorld.o
	$(CC) $(CFLAGS) -O0 -o $@ $^ -lX11 -lm -I/usr/local/include -L/usr/local/lib -lSDL

world.o : world.cpp world.hpp graphics.h perlin.h util.h bot.hpp loc.hpp

graphics.o : graphics.c graphics.h util.h

anim.o : anim.c anim.h graphics.h util.h

perlin.o : perlin.c perlin.h util.h

main.o : main.cpp graphics.h anim.h world.hpp util.h resourceGraph.hpp

util.o : util.c util.h

bot.o : bot.cpp bot.hpp world.hpp graphics.h loc.hpp navigate.hpp innerWorld.hpp

resourceGraph.o : resourceGraph.cpp resourceGraph.hpp

loc.o : loc.cpp loc.hpp util.h

navigate.o : navigate.cpp navigate.hpp loc.hpp innerWorld.hpp

innerWorld.o : innerWorld.cpp innerWorld.hpp world.hpp bot.hpp

clean:
	rm -f main.o graphics.o anim.o world.o perlin.o util.o bot.o resourceGraph.o loc.o innerWorld.o navigate.o
