#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdio.h>

namespace SDL_setting
{
#define SCRX 750 // screen width
#define SCRY 600 // screen height
#define background_color RGBto
#define textbar_color_up RGBtoInt(255, 198, 120)
#define textbar_color_down RGBtoInt(220, 55, 15)
#define textbar_textcolor RGBAcolor(255, 188, 72, 0)
	const int margins = 8;
	const int squaresize = 32;
	SDL_Window *screen;
	SDL_Renderer *renderer;
	unsigned int *pixptr;
	// music
	Mix_Music *backgroundMusic = NULL;

	Mix_Chunk *WallEffect = NULL;
	Mix_Chunk *PawnEffect = NULL;
	// font
	TTF_Font *font = NULL;
	// Button
	const int BUTTON_WIDTH = 150;
	const int BUTTON_HEIGHT = 70;
	enum ButtonSprite
	{
		BUTTON_SPRITE_MOUSE_OUT = 0,
		BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
		BUTTON_SPRITE_MOUSE_DOWN = 2,
		BUTTON_SPRITE_MOUSE_UP = 3,
		BUTTON_SPRITE_TOTAL = 4
	};
#define button_down_color RGBtoInt(255, 70, 0)
#define button_up_color RGBtoInt(255, 200, 0)
#define button_text_color RGBAcolor(240, 250, 215, 0)
	// Init
	inline bool init();

	// drawing functions
	inline void putpixel(int x, int y, int color);
	inline int RGBtoInt(int r, int g, int b);
	inline void draw_square(int x, int y, int width, int height, int color);
	inline int InttoR(int color);
	inline int InttoG(int color);
	inline int InttoB(int color);
	inline void grad_square(int x, int y, int width, int height, int color1, int color2);
	inline SDL_Color RGBAcolor(int r, int g, int b, int a);
	// load functions
	SDL_Surface *load_image(std::string filename);
	void loadMusic();
	// Implementation
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
		SDL_Color clr = {r, g, b, a};
		return clr;
	}

	inline void putpixel(int x, int y, int color)
	{
		int lineoffset = y * (SDL_GetWindowSurface(screen)->pitch / 4);
		pixptr[lineoffset + x] = color;
	}

	SDL_Surface *load_image(std::string filename)
	{
		SDL_Surface *loadedImage = NULL;
		SDL_Surface *optimizedImage = NULL;
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
		backgroundMusic = Mix_LoadMUS("src/backgroundMusic.wav");
		Mix_VolumeMusic(20);
		WallEffect = Mix_LoadWAV("src/Wall_soundEffect.wav");
		Mix_VolumeChunk(WallEffect, 20000);
		PawnEffect = Mix_LoadWAV("src/Pawn_soundEffect.wav");
		return;
	}
	bool init()
	{
		// Initialization flag
		bool success = true;

		// Initialize SDL
		atexit(SDL_Quit);
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)

		{
			fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
			exit(1);
		}

		screen = SDL_CreateWindow("Quoridor game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCRX, SCRY, SDL_WINDOW_SHOWN);
		if (screen == NULL)
		{
			fprintf(stderr, "Unable to set 800x600 video: %s\n", SDL_GetError());
			exit(1);
		}

		else
		{
			// Create vsynced renderer for window
			renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				// initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				}
				loadMusic();
				// Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
		return success;
	}
}
