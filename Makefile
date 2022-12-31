all: common.h player.h wall.h game.h main.cpp
	g++ -I src/include -L src/lib -o main common.h player.h wall.h game.h Text.cpp main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer