CC = clang++-6.0
CFLAGS = -Wall -std=c++17 -g -lstdc++fs

Rep : main.o graphics.o anim.o world.o perlin.o util.o bot.o resourceGraph.o loc.o navigate.o innerWorld.o direction.o block.o timer.o
	$(CC) $(CFLAGS) -pthread -O0 -o $@ $^ -lX11 -lm -I/usr/local/include -L/usr/local/lib -lSDL

world.o : world.cpp world.hpp graphics.h perlin.h util.hpp bot.hpp loc.hpp anim.hpp block.hpp timer.hpp

graphics.o : graphics.c graphics.h util.hpp direction.hpp

anim.o : anim.cpp anim.hpp graphics.h util.hpp direction.hpp

perlin.o : perlin.c perlin.h util.hpp

main.o : main.cpp graphics.h anim.hpp world.hpp util.hpp resourceGraph.hpp anim.hpp

util.o : util.cpp util.hpp

bot.o : bot.cpp bot.hpp world.hpp graphics.h loc.hpp navigate.hpp innerWorld.hpp anim.hpp direction.hpp
bot.cpp : util.hpp

resourceGraph.o : resourceGraph.cpp resourceGraph.hpp

loc.o : loc.cpp loc.hpp util.hpp

navigate.o : navigate.cpp navigate.hpp loc.hpp innerWorld.hpp

innerWorld.o : innerWorld.cpp innerWorld.hpp world.hpp bot.hpp

direction.o : direction.hpp

block.o : block.hpp block.cpp anim.hpp

clean:
	rm -f main.o graphics.o anim.o world.o perlin.o util.o bot.o resourceGraph.o loc.o innerWorld.o navigate.o direction.o block.o timer.o
