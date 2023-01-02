#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include "common.h"
#include "game.h"
#include "wall.h"

using namespace SDL_setting;
class Player
{
	SDL_Surface *playerlook, *playerlooksel;
	SDL_Rect entrypoint;

public:
	Wall walls[10];
	int desty, locx, locy, wselected; // wselected -1 none, 0-9 blocks, 10 pawn
	bool mouse_over;
	void reset_player()
	{
		mouse_over = false;
		locy = 10 - desty;
		wselected = -1;
		locx = 5;
		entrypoint.x = 200 + 4 * margins + 4 * squaresize;
		entrypoint.y = squaresize + 60 + locy * (squaresize + margins);
	}
	Player(int sy)
	{
		desty = 10 - sy;
		reset_player();
		if (sy == 9)
		{
			playerlook = load_image("src/pixil-frame-0.png");
			playerlooksel = load_image("src/pixil-frame-0.png");
		}
		else if (sy == 1)
		{
			playerlook = load_image("src/pixil-frame-1.png");
			playerlooksel = load_image("src/pixil-frame-1.png");
		}
	}
	~Player()
	{
		SDL_FreeSurface(playerlook);
		SDL_FreeSurface(playerlooksel);
	}
	bool can_up(Player *enemy)
	{
		for (int i = 0; i < 10; i++)
			if ((!enemy->walls[i].vertical) and enemy->walls[i].placed)
				if (enemy->walls[i].spy + 1 == this->locy)
					if (this->locx - 1 == enemy->walls[i].spx or this->locx - 2 == enemy->walls[i].spx)
						return false;
		for (int i = 0; i < 10; i++)
			if ((!this->walls[i].vertical) and this->walls[i].placed)
				if (this->walls[i].spy + 1 == this->locy)
					if (this->locx - 1 == this->walls[i].spx or this->locx - 2 == this->walls[i].spx)
						return false;
		if (locy - 1 > 0)
			return true;
		return false;
	}
	bool can_left(Player *enemy)
	{
		for (int i = 0; i < 10; i++)
			if ((enemy->walls[i].vertical) and enemy->walls[i].placed)
				if (enemy->walls[i].spx + 1 == this->locx)
					if (this->locy - 1 == enemy->walls[i].spy or this->locy == enemy->walls[i].spy)
						return false;
		for (int i = 0; i < 10; i++)
			if ((this->walls[i].vertical) and this->walls[i].placed)
				if (this->walls[i].spx + 1 == this->locx)
					if (this->locy - 1 == this->walls[i].spy or this->locy == this->walls[i].spy)
						return false;
		if (locx - 1 > 0)
			return true;
		return false;
	}
	bool can_down(Player *enemy)
	{
		for (int i = 0; i < 10; i++)
			if ((!enemy->walls[i].vertical) and enemy->walls[i].placed)
				if (enemy->walls[i].spy == this->locy)
					if (this->locx - 1 == enemy->walls[i].spx or this->locx - 2 == enemy->walls[i].spx)
						return false;
		for (int i = 0; i < 10; i++)
			if ((!this->walls[i].vertical) and this->walls[i].placed)
				if (this->walls[i].spy == this->locy)
					if (this->locx - 1 == this->walls[i].spx or this->locx - 2 == this->walls[i].spx)
						return false;
		if (locy + 1 < 10)
			return true;
		return false;
	}
	bool can_right(Player *enemy)
	{
		for (int i = 0; i < 10; i++)
			if ((enemy->walls[i].vertical) and enemy->walls[i].placed)
				if (enemy->walls[i].spx == this->locx)
					if (this->locy - 1 == enemy->walls[i].spy or this->locy == enemy->walls[i].spy)
						return false;
		for (int i = 0; i < 10; i++)
			if ((this->walls[i].vertical) and this->walls[i].placed)
				if (this->walls[i].spx == this->locx)
					if (this->locy - 1 == this->walls[i].spy or this->locy == this->walls[i].spy)
						return false;
		if (locx + 1 < 10)
			return true;
		return false;
	}
	void select(Player *enemy)
	{
		if (wselected >= 0 and wselected < 10)
		{
			walls[wselected].draw_wall();
			wselected = -1;
		}
		wselected = 9 - wselected;
		draw_player();

		if (wselected == 10)
		{
			make_around(enemy);
		}
		else
		{
			del_around(enemy);
		}
		SDL_Rect temp;
		temp.x = 0;
		temp.y = 0;
		temp.w = SCRX;
		temp.h = SCRY;
		SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
	}
	void make_around(Player *enemy)
	{
		SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 0);
		SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, playerlooksel);

		SDL_SetTextureAlphaMod(text, 80);
		if (can_left(enemy))
		{
			if (enemy->locx + 1 == this->locx and enemy->locy == this->locy)
			{
				if (enemy->can_left(this))
				{
					entrypoint.x = (locx - 3) * (squaresize + margins) + 200;
					entrypoint.y = squaresize + 60 + (locy) * (squaresize + margins);
					draw_player();
				}
				else
				{
					if (enemy->can_up(this))
					{
						entrypoint.x = (locx - 2) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy - 1) * (squaresize + margins);
						draw_player();
					}
					if (enemy->can_down(this))
					{
						entrypoint.x = (locx - 2) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy + 1) * (squaresize + margins);
						draw_player();
					}
				}
			}
			else
			{
				entrypoint.y = squaresize + 60 + (locy) * (squaresize + margins);
				entrypoint.x = (locx - 2) * (squaresize + margins) + 200;
				draw_player();
			}
		}
		if (can_right(enemy))
		{
			if (enemy->locx - 1 == this->locx and enemy->locy == this->locy)
			{
				if (enemy->can_right(this))
				{
					entrypoint.x = (locx + 1) * (squaresize + margins) + 200;
					entrypoint.y = squaresize + 60 + (locy) * (squaresize + margins);
					draw_player();
				}
				else
				{
					if (enemy->can_up(this))
					{
						entrypoint.x = (locx) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy - 1) * (squaresize + margins);
						draw_player();
					}
					if (enemy->can_down(this))
					{
						entrypoint.x = (locx) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy + 1) * (squaresize + margins);
						draw_player();
					}
				}
			}
			else
			{
				entrypoint.y = squaresize + 60 + (locy) * (squaresize + margins);
				entrypoint.x = (locx) * (squaresize + margins) + 200;
				draw_player();
			}
		}
		if (can_up(enemy))
		{
			if (enemy->locx == this->locx and enemy->locy == this->locy - 1)
			{
				if (enemy->can_up(this))
				{
					entrypoint.x = (locx - 1) * (squaresize + margins) + 200;
					entrypoint.y = squaresize + 60 + (locy - 2) * (squaresize + margins);
					draw_player();
				}
				else
				{
					if (enemy->can_right(this))
					{
						entrypoint.x = (locx) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy - 1) * (squaresize + margins);
						draw_player();
					}
					if (enemy->can_left(this))
					{
						entrypoint.x = (locx - 2) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy - 1) * (squaresize + margins);
						draw_player();
					}
				}
			}
			else
			{
				entrypoint.x = (locx - 1) * (squaresize + margins) + 200;
				entrypoint.y = squaresize + 60 + (locy - 1) * (squaresize + margins);
				draw_player();
			}
		}
		if (can_down(enemy))
		{
			if (enemy->locx == this->locx and enemy->locy - 1 == this->locy)
			{
				if (enemy->can_down(this))
				{
					entrypoint.x = (locx - 1) * (squaresize + margins) + 200;
					entrypoint.y = squaresize + 60 + (locy + 2) * (squaresize + margins);
					draw_player();
				}
				else
				{
					if (enemy->can_right(this))
					{
						entrypoint.x = (locx) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy + 1) * (squaresize + margins);
						draw_player();
					}
					if (enemy->can_left(this))
					{
						entrypoint.x = (locx - 2) * (squaresize + margins) + 200;
						entrypoint.y = squaresize + 60 + (locy + 1) * (squaresize + margins);
						draw_player();
					}
				}
			}
			else
			{
				entrypoint.x = (locx - 1) * (squaresize + margins) + 200;
				entrypoint.y = squaresize + 60 + (locy + 1) * (squaresize + margins);
				draw_player();
			}
		}
		entrypoint.x = (locx - 1) * (squaresize + margins) + 200;
		entrypoint.y = squaresize + 60 + locy * (squaresize + margins);

		text = SDL_CreateTextureFromSurface(renderer, playerlooksel);

		SDL_SetTextureAlphaMod(text, 255);
	}
	void del_around(Player *enemy)
	{
		if (can_left(enemy))
		{
			if (enemy->locx + 1 == this->locx and enemy->locy == this->locy)
			{
				if (enemy->can_left(this))
				{
					draw_square((locx - 3) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 1) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
				}
				else
				{
					if (enemy->can_up(this))
					{
						draw_square((locx - 2) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 2) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
					if (enemy->can_down(this))
					{
						draw_square((locx - 2) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
				}
			}
			else
			{
				draw_square((locx - 2) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 1) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
			}
		}
		if (can_right(enemy))
		{
			if (enemy->locx - 1 == this->locx and enemy->locy == this->locy)
			{
				if (enemy->can_right(this))
				{
					draw_square((locx + 1) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 1) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
				}
				else
				{
					if (enemy->can_up(this))
					{
						draw_square((locx) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 2) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
					if (enemy->can_down(this))
					{
						draw_square((locx) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
				}
			}
			else
			{
				draw_square((locx) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 1) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
			}
		}
		if (can_up(enemy))
		{
			if (enemy->locx == this->locx and enemy->locy == this->locy - 1)
			{
				if (enemy->can_up(this))
				{
					draw_square((locx - 1) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 3) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
				}
				else
				{
					if (enemy->can_right(this))
					{
						draw_square((locx) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 2) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
					if (enemy->can_left(this))
					{
						draw_square((locx - 2) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 2) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
				}
			}
			else
			{
				draw_square((locx - 1) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 2) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
			}
		}
		if (can_down(enemy))
		{
			if (enemy->locx == this->locx and enemy->locy - 1 == this->locy)
			{
				if (enemy->can_down(this))
				{
					draw_square((locx - 1) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy + 1) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
				}
				else
				{
					if (enemy->can_right(this))
					{
						draw_square((locx) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
					if (enemy->can_left(this))
					{
						draw_square((locx - 2) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
					}
				}
			}
			else
			{
				draw_square((locx - 1) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
			}
		}
	}
	void draw_player()
	{
		SDL_Surface *winSurface = SDL_GetWindowSurface(screen);
		if (wselected == 10)
			SDL_BlitSurface(playerlooksel, NULL, winSurface, &entrypoint);
		else
			SDL_BlitSurface(playerlook, NULL, winSurface, &entrypoint);
	}
	bool onplayer(int x, int y)
	{
		if (x > (entrypoint.x) and x < (entrypoint.x + squaresize))
			if (y > (entrypoint.y) and y < (entrypoint.y + squaresize))
				return true;
		return false;
	}
	bool onsquare(int x, int y, int bx, int by)
	{
		if (x > bx and x < bx + squaresize)
			if (y > by and y < by + squaresize)
				return true;
		return false;
	}
	int possible_move(int x, int y, Player *enemy)
	{
		if (can_up(enemy))
		{
			if (enemy->locx == this->locx and enemy->locy == this->locy - 1)
			{
				if (enemy->can_up(this))
				{
					if (onsquare(x, y, (locx - 1) * (squaresize + margins) + 200, squaresize + 60 + (locy - 2) * (squaresize + margins)))
					{
						return move(0, -2);
					}
				}
				else
				{
					if (enemy->can_right(this))
					{
						if (onsquare(x, y, (locx) * (squaresize + margins) + 200, squaresize + 60 + (locy - 1) * (squaresize + margins)))
						{
							return move(1, -1);
						}
					}
					if (enemy->can_left(this))
					{
						if (onsquare(x, y, (locx - 2) * (squaresize + margins) + 200, squaresize + 60 + (locy - 1) * (squaresize + margins)))
						{
							return move(-1, -1);
						}
					}
				}
			}
			else
			{
				if (onsquare(x, y, (locx - 1) * (squaresize + margins) + 200, squaresize + 60 + (locy - 1) * (squaresize + margins)))
				{
					return move(0, -1);
				}
			}
		}
		if (can_left(enemy))
		{
			if (enemy->locx + 1 == this->locx and enemy->locy == this->locy)
			{
				if (enemy->can_left(this))
				{
					if (onsquare(x, y, (locx - 3) * (squaresize + margins) + 200, squaresize + 60 + (locy) * (squaresize + margins)))
					{
						return move(-2, 0);
					}
				}
				else
				{
					if (enemy->can_up(this))
					{
						if (onsquare(x, y, (locx - 2) * (squaresize + margins) + 200, squaresize + 60 + (locy - 1) * (squaresize + margins)))
						{
							return move(-1, -1);
						}
					}
					if (enemy->can_down(this))
					{
						if (onsquare(x, y, (locx - 2) * (squaresize + margins) + 200, squaresize + 60 + (locy + 1) * (squaresize + margins)))
						{
							return move(-1, 1);
						}
					}
				}
			}
			else
			{
				if (onsquare(x, y, (locx - 2) * (squaresize + margins) + 200, squaresize + 60 + (locy) * (squaresize + margins)))
				{
					return move(-1, 0);
				}
			}
		}
		// right
		if (can_right(enemy))
		{
			if (enemy->locx - 1 == this->locx and enemy->locy == this->locy)
			{
				if (enemy->can_right(this))
				{
					if (onsquare(x, y, (locx + 1) * (squaresize + margins) + 200, squaresize + 60 + (locy) * (squaresize + margins)))
					{
						return move(2, 0);
					}
				}
				else
				{
					if (enemy->can_up(this))
					{
						if (onsquare(x, y, (locx) * (squaresize + margins) + 200, squaresize + 60 + (locy - 1) * (squaresize + margins)))
						{
							return move(1, -1);
						}
					}
					if (enemy->can_down(this))
					{
						if (onsquare(x, y, (locx) * (squaresize + margins) + 200, squaresize + 60 + (locy + 1) * (squaresize + margins)))
						{
							return move(1, 1);
						}
					}
				}
			}
			else
			{
				if (onsquare(x, y, (locx) * (squaresize + margins) + 200, squaresize + 60 + (locy) * (squaresize + margins)))
				{
					return move(1, 0);
				}
			}
		}
		// down
		if (can_down(enemy))
		{
			if (enemy->locx == this->locx and enemy->locy - 1 == this->locy)
			{
				if (enemy->can_down(this))
				{
					if (onsquare(x, y, (locx - 1) * (squaresize + margins) + 200, squaresize + 60 + (locy + 2) * (squaresize + margins)))
					{
						return move(0, 2);
					}
				}
				else
				{
					if (enemy->can_right(this))
					{
						if (onsquare(x, y, (locx) * (squaresize + margins) + 200, squaresize + 60 + (locy + 1) * (squaresize + margins)))
						{
							return move(1, 1);
						}
					}
					if (enemy->can_left(this))
					{
						if (onsquare(x, y, (locx - 2) * (squaresize + margins) + 200, squaresize + 60 + (locy + 1) * (squaresize + margins)))
						{
							return move(-1, 1);
						}
					}
				}
			}
			else
			{
				if (onsquare(x, y, (locx - 1) * (squaresize + margins) + 200, squaresize + 60 + (locy + 1) * (squaresize + margins)))
				{
					return move(0, 1);
				}
			}
		}
		return 0;
	}
	int move(int addx, int addy)
	{
		draw_square((locx - 1) * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + (locy - 1) * (squaresize + margins), squaresize, squaresize, RGBtoInt(59, 19, 2));
		locx += addx;
		locy += addy;
		entrypoint.x = (locx - 1) * (squaresize + margins) + 200;
		entrypoint.y = squaresize + 60 + locy * (squaresize + margins);
		// pawn sound effect
		Mix_PlayChannel(-1, PawnEffect, 0);
		draw_player();
		wselected = -1;
		if (locy == desty) // game end
			return -1;
		return 1;
	}
};