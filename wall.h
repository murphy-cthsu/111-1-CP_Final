#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include "common.h"
#include "player.h"
#include "game.h"
using namespace SDL_setting;

class Wall
{
public:
	int spx, spy;
	bool placed, vertical;
	void set_wall(int spx, int spy)
	{
		this->placed = false;
		this->vertical = true;
		this->spx = spx;
		this->spy = spy;
	}
	void set_focus()
	{
		draw_wall(RGBtoInt(200, 70, 50));
	}
	void draw_wall(int color)
	{
		SDL_Rect temp;
		if (spy == 0)
		{
			draw_square(spx * (squaresize + margins) - margins + 200, 60, margins, margins + 2 * squaresize, color);
			temp.x = spx * (squaresize + margins) - margins + 200;
			temp.y = 60;
			temp.w = margins;
			temp.h = margins + 2 * squaresize;
			SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
		}
		else
		{
			if (spy == 9)
			{
				draw_square(spx * (squaresize + margins) - margins + 200, squaresize * 11 + 60 + 9 * margins, margins, margins + 2 * squaresize, color);
				temp.x = spx * (squaresize + margins) - margins + 200;
				temp.y = squaresize * 2 + 60 + spy * (squaresize + margins);
				temp.w = margins;
				temp.h = margins + 2 * squaresize;
				SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
			}
			else
			{
				if (vertical)
				{
					draw_square(spx * (squaresize + margins) - margins + 200, squaresize + 60 + spy * (squaresize + margins), margins, margins + 2 * squaresize, color);
					temp.x = spx * (squaresize + margins) - margins + 200;
					temp.y = squaresize + 60 + spy * (squaresize + margins);
					temp.w = margins;
					temp.h = margins + 2 * squaresize;
					SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
				}
				else
				{
					draw_square(spx * (squaresize + margins) + 200, squaresize * 2 + 60 + spy * (squaresize + margins), margins + 2 * squaresize, margins, color);

					temp.x = spx * (squaresize + margins) + 200;
					temp.y = squaresize * 2 + 60 + spy * (squaresize + margins);
					temp.w = margins + 2 * squaresize;
					temp.h = margins;
					SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
				}
			}
		}
	}
	void draw_wall()
	{
		draw_wall(RGBtoInt(190, 130, 60));
	}
	void changepos(int spx, int spy, bool vertical)
	{
		draw_wall(RGBtoInt(120, 45, 13));
		this->spx = spx;
		this->spy = spy;
		this->vertical = vertical;
		this->placed = true;
		// wall sound effect
		Mix_PlayChannel(-1, WallEffect, 0);
		draw_wall();
	}
};