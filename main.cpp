// update renderer
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include "common.h"

#include "game.h"
#include "wall.h"
#include "player.h"
#include "Text.h"
using namespace SDL_setting;

int main(int argc, char *argv[])
{

	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)

	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	screen = SDL_CreateWindow("Quoridor game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCRX, SCRY, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (screen == NULL)
	{
		fprintf(stderr, "Unable to set 800x600 video: %s\n", SDL_GetError());
		exit(1);
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	loadMusic();

	pixptr = (unsigned int *)SDL_GetWindowSurface(screen)->pixels;
	if (TTF_Init() == -1)
	{
		cout << "ttf couldn't be initialized.";
		exit(1);
	}
	Game *main_game = new Game();
	// Textbar *test_text = new Textbar(20, 35, 100, 40, "hiiiiiii");
	//Textbar *turn_text = new Textbar(20, 200, 250, 50, "Now Playing:P1");
	// Textbar *wall_left = new Textbar(20, 200, 180, 50, "Wall Left:");
	int menucevt;
	SDL_Rect temp;
	temp.x = 2;
	temp.y = 0;
	temp.w = 250;
	temp.h = SCRY;
	SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
	bool move_event_menu = false;
	while (1)
	{
		SDL_Delay(1);
		SDL_Event event;
		// If there is no mi=usic playing
		if (Mix_PlayingMusic() == 0)
		{
			// Play the music
			Mix_PlayMusic(backgroundMusic, -1);
		}
		while (SDL_PollEvent(&event))
		{

			switch (event.type)
			{
			case SDL_MOUSEBUTTONUP:
				if (event.button.y >= 42)
				{
					main_game->click_event(event.button.button, event.button.x, event.button.y);
				}
				/*if (main_game->turn)
				{
					turn_text->show_text("Now Playing:P1");
				}
				else
					turn_text->show_text("Now Playing:P2");
				if (main_game->game_end)
				{
					if (main_game->turn)
					{
						turn_text->show_text("P1 Wins");
						// delete turn_text;
						// turn_text = new Textbar(20, 200, 250, 50, "P1 Wins");
					}
					else
					{
						turn_text->show_text("P2 Wins");
					}
				}*/
				break;
			case SDL_MOUSEMOTION:
				if (event.motion.y >= 42)
				{
					if (event.motion.x >= 254)
					{
						main_game->move_event(event.motion.x, event.motion.y);
					}
				}
				break;
			case SDL_QUIT:
			quit:
				delete main_game;
				return (0);
			}
		}
	}
	return 0;
}
