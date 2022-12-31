#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <stdio.h>

namespace SDL_setting {
#define SCRX 750
#define SCRY 600
	const int margins = 8;
	const int squaresize = 32;
	SDL_Window* screen;
	unsigned int* pixptr;
	Mix_Music* backgroundMusic = NULL;
	Mix_Chunk* WallEffect = NULL;
	Mix_Chunk* PawnEffect = NULL;

	inline void putpixel(int x, int y, int color);
	inline int RGBtoInt(int r, int g, int b);
	inline void draw_square(int x, int y, int width, int height, int color);
	inline int InttoR(int color);
	inline int InttoG(int color);
	inline int InttoB(int color);
	inline void grad_square(int x, int y, int width, int height, int color1, int color2);
	inline SDL_Color RGBAcolor(int r, int g, int b, int a);
	SDL_Surface* load_image(std::string filename);
	void loadMusic();

	inline void draw_linex(int x, int ly, int width, int color)
	{
		int lx = x + width;
		while (lx > x)
		{
			lx--;
			putpixel(lx, ly, color);
		}
	}

	inline void draw_square(int x, int y, int width, int height, int color)
	{
		int ly = y + height;
		while (ly > y)
		{
			ly--;
			draw_linex(x, ly, width, color);
		}
	}

	void grad_square(int x, int y, int width, int height, int color1, int color2)
	{
		int ly = y + height;
		int r = int((InttoR(color2) - InttoR(color1)) / (float)(height)-0.1), g = int((InttoG(color2) - InttoG(color1)) / (float)(height)-0.1), b = int((InttoB(color2) - InttoB(color1)) / (float)(height)-0.1);

		while (ly > y)
		{
			ly--;
			draw_linex(x, ly, width, color1);
			color1 = RGBtoInt(InttoR(color1) + r, InttoG(color1) + g, InttoB(color1) + b);
		}
	}

	inline int RGBtoInt(int r, int g, int b)
	{
		return ((r << 16) | (g << 8) | b);
	}
	inline int InttoR(int color)
	{
		return (color >> 16);
	}
	inline int InttoG(int color)
	{
		return (color >> 8) % 256;
	}
	inline int InttoB(int color)
	{
		return (color) % 256;
	}

	inline SDL_Color RGBAcolor(int r, int g, int b, int a)
	{
		SDL_Color clr = { r,g,b,a };
		return clr;
	}

	inline void putpixel(int x, int y, int color)
	{
		int lineoffset = y * (SDL_GetWindowSurface(screen)->pitch / 4);
		pixptr[lineoffset + x] = color;
	}

	SDL_Surface* load_image(std::string filename)
	{
		SDL_Surface* loadedImage = NULL;
		SDL_Surface* optimizedImage = NULL;
		loadedImage = IMG_Load(filename.c_str());

		if (loadedImage != NULL)
		{
			SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0, 0, 0));
			optimizedImage = SDL_ConvertSurface(loadedImage, SDL_GetWindowSurface(screen)->format, 0);
			SDL_FreeSurface(loadedImage);
		}
		return optimizedImage;
	}

	void loadMusic()
	{
		backgroundMusic = Mix_LoadMUS("C:/Users/User/source/repos/SDLWindow/music/backgroundMusic.wav");
		WallEffect = Mix_LoadWAV("C:/Users/User/source/repos/SDLWindow/music/Wall_soundEffect.wav");
		PawnEffect = Mix_LoadWAV("C:/Users/User/source/repos/SDLWindow/music/Pawn_soundEffect.wav");
		return;
	}
}

