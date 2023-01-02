all: common.h player.h wall.h game.h Text.h Button.h lazy.ttf main.cpp
	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 