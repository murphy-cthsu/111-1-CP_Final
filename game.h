#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include "common.h"
#include "player.h"
#include "wall.h"
using namespace SDL_setting;

class Game
{
	Wall demo;
	bool wall_canplace(int spx, int spy, bool vertical)
	{
		for (int i = 0; i < 10; i++)
			if (p2->walls[i].placed)
			{
				if (p2->walls[i].vertical == vertical)
				{
					if (p2->walls[i].spy == spy and p2->walls[i].spx == spx)
						return false;
					if (p2->walls[i].spx == spx and vertical)
					{
						if (p2->walls[i].spy == spy - 1 or p2->walls[i].spy == spy + 1)
							return false;
					}
					else
					{
						if (p2->walls[i].spx == spx + 1 or p2->walls[i].spx == spx - 1)
							if (p2->walls[i].spy == spy and (!vertical))
								return false;
					}
				}
				else // cross
				{
					if (p2->walls[i].vertical and p2->walls[i].spx == spx + 1 and p2->walls[i].spy == spy)
						return false;
					if (vertical and spx == p2->walls[i].spx + 1 and spy == p2->walls[i].spy)
						return false;
				}
			}
		for (int i = 0; i < 10; i++)
			if (p1->walls[i].placed)
			{
				if (p1->walls[i].vertical == vertical)
				{
					if (p1->walls[i].spy == spy and p1->walls[i].spx == spx)
						return false;
					if (p1->walls[i].spx == spx and vertical)
					{
						if (p1->walls[i].spy == spy - 1 or p1->walls[i].spy == spy + 1)
							return false;
					}
					else
					{
						if (p1->walls[i].spx == spx + 1 or p1->walls[i].spx == spx - 1)
							if (p1->walls[i].spy == spy and (!vertical))
								return false;
					}
				}
				else
				{
					if (p1->walls[i].vertical and p1->walls[i].spx == spx + 1 and p1->walls[i].spy == spy)
						return false;
					if (vertical and spx == p1->walls[i].spx + 1 and spy == p1->walls[i].spy)
						return false;
				}
			}
		return true;
	}
	void change_wall_place(int mx, int my, Wall *swall)
	{

		if (demo.spy != 0)
		{
			swall->changepos(demo.spx, demo.spy, demo.vertical);
			if (turn)
			{
				p1->wselected = -1;
				change_turn();
			}
			else
			{
				p2->wselected = -1;
				change_turn();
			}
			demo.spy = 0;
		}
	}
	void preview_wall_place(int mx, int my)
	{
		Wall temp = demo;
		if (wall_place(mx, my, &(demo.spx), &(demo.spy), &(demo.vertical)))
		{
			if (!(temp.spx == demo.spx and temp.spy == demo.spy and temp.vertical == demo.vertical))
			{
				if (temp.spy != 0)
					temp.draw_wall(RGBtoInt(190, 130, 60));
				demo.draw_wall(RGBtoInt(230, 110, 50));
			}
		}
		else
		{
			demo.spy = 0;
			if (temp.spy != 0)
				temp.draw_wall(RGBtoInt(190, 130, 60));
		}
	}
	bool wall_place(int mx, int my, int *spx, int *spy, bool *vertical)
	{
		mx += margins - 200;
		*spx = mx / (squaresize + margins);
		mx %= (squaresize + margins);

		my -= squaresize * 2;
		my -= 60;
		*spy = my / (squaresize + margins) + 1;
		my %= (squaresize + margins);
		if (my < margins)
		{
			if (mx >= margins)
			{
				*vertical = false;
				*spy -= 1;
				if (mx < margins + squaresize / 2)
					if (*spx > 0)
						*spx -= 1;
			}
			else if (*vertical)
				*spy -= 1;
			else
			{
				*spx -= 1;
				*spy -= 1;
			}
		}
		else if (mx < margins)
		{
			*vertical = true;
			if (my < margins + squaresize / 2)
				if (*spy > 1)
					*spy -= 1;
		}
		else
			return false;
		if (!wall_canplace((*spx), (*spy), (*vertical))) // over existing block or implaceable
			return false;
		if (*vertical)
		{
			if ((*spx) > 0 and (*spx) <= 8 and (*spy) > 0 and (*spy) <= 8)
				return true;
		}
		else
		{
			if (((*spx) < 8) and ((*spy) > 0) and ((*spy) <= 8))
				return true;
		}
		return false;
	}
	void change_turn()
	{
		turn = !turn;
	}
	Player *p2, *p1;

public:
	bool turn, game_end; // p1 true, p2 false
	Game()
	{
		reset_game_status();
		p2 = new Player(1);
		p1 = new Player(9);
		set_walls();
		draw_board();
		demo.set_wall(1, 0);
	}
	void reset_game_status()
	{
		game_end = false;
		turn = true;
	}
	void new_game()
	{
		reset_game_status();
		reset_players();
		set_walls();
		draw_board();
		demo.set_wall(1, 0);
	}
	void reset_players()
	{
		p2->reset_player();
		p1->reset_player();
	}
	void set_walls()
	{
		for (int i = 0; i < 10; i++)
			p2->walls[i].set_wall(i, 0);
		for (int i = 0; i < 10; i++)
			p1->walls[i].set_wall(i, 9);
	}
	void draw_board()
	{
		int i, j;
		for (j = 0; j < 9; j++)
			for (i = 0; i < 9; i++)
				draw_square(i * (squaresize + margins) + 200, squaresize * 2 + margins + 60 + j * (squaresize + margins), squaresize, squaresize, RGBtoInt(230, 170, 100));
		for (i = 0; i < 10; i++)
			draw_square(i * (squaresize + margins) + 200 - margins, squaresize * 2 + margins + 60, margins, squaresize * 9 + margins * 8, RGBtoInt(190, 130, 60));
		for (i = 0; i < 8; i++)
			draw_square(200, i * (squaresize + margins) + squaresize * 3 + margins + 60, squaresize * 9 + margins * 8, margins, RGBtoInt(190, 130, 60));
		draw_square(200 - margins, 60, squaresize * 9 + margins * 10, squaresize * 2 + margins, RGBtoInt(190, 130, 60));
		draw_square(200 - margins, 60 + squaresize * 11 + margins * 9, squaresize * 9 + margins * 10, squaresize * 2 + margins, RGBtoInt(190, 130, 60));
		for (i = 0; i < 10; i++)
			p2->walls[i].draw_wall();
		for (i = 0; i < 10; i++)
			p1->walls[i].draw_wall();
		p1->draw_player();
		p2->draw_player();
		if (p1->wselected == 10)
			p1->make_around(p2);
		else if (p2->wselected == 10)
			p2->make_around(p1);

		SDL_Rect temp;
		temp.x = 0;
		temp.y = 0;
		temp.w = SCRX;
		temp.h = SCRY;
		SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
	}
	int which_wall(int x, bool turn)
	{
		x += margins - 200;
		int spx = x / (squaresize + margins);
		x %= (squaresize + margins);
		if (x < margins and spx >= 0 and spx <= 9)
			if (turn)
			{
				if (!p1->walls[spx].placed)
					return spx;
			}
			else if (!p2->walls[spx].placed)
				return spx;
		return -1;
	}
	void move_event(int x, int y)
	{
		if (!game_end)
		{
			if (turn)
			{
				if (p1->onplayer(x, y) and !(p1->mouse_over))
				{

					if (p1->wselected != 10)
					{
						p1->mouse_over = true;
					}
				}

				else
				{
					if (!p1->onplayer(x, y) and (p1->mouse_over))
					{
						p1->mouse_over = false;
					}
					if (p1->wselected >= 0 and p1->wselected < 10)
						preview_wall_place(x, y);
				}
			}
			else if (p2->onplayer(x, y) and !(p2->mouse_over))
			{
				if (p2->wselected != 10)
				{
					p2->mouse_over = true;
				}
			}
			else
			{
				if ((!p2->onplayer(x, y)) and (p2->mouse_over))
				{
					p2->mouse_over = false;
				}
				if (p2->wselected >= 0 and p2->wselected < 10)
					preview_wall_place(x, y);
			}
		}
	}
	void click_event(int button, int x, int y)
	{
		// walls[0][0].changepos(4,2,false);
		int other_lx, other_ly;
		if (!game_end)
			if (turn)
			{
				other_lx = p2->locx;
				other_ly = p2->locy;
				if (p1->onplayer(x, y))
					p1->select(p2);
				else
				{
					if (p1->wselected == 10)
					{
						p1->select(p2);
						int possible = p1->possible_move(x, y, p2);
						if (possible == -1)
						{
							game_end = true;
						}
						else if (possible != 0)
							change_turn();
					}
					else
					{
						if (p1->wselected >= 0 and p1->wselected < 10)
						{
							change_wall_place(x, y, &(p1->walls[p1->wselected]));
						}
						else if (y >= 60 + 11 * squaresize + 9 * margins and y < 60 + 13 * squaresize + 10 * margins)
						{
							int spx = which_wall(x, turn);
							if (spx < 0)
								return;
							if (p1->wselected != spx and p1->wselected >= 0 and p1->wselected < 10)
								p1->walls[p1->wselected].draw_wall();
							p1->wselected = spx;
							p1->walls[spx].set_focus();
						}
					}
				}
			}
			else
			{
				if (p2->onplayer(x, y))
					p2->select(p1);
				else
				{
					if (p2->wselected == 10)
					{
						p2->select(p1);
						int possible;
						possible = p2->possible_move(x, y, p1);
						if (possible == -1)
						{
							game_end = true;
						}
						else if (possible != 0)
							change_turn();
					}
					else
					{
						if (p2->wselected >= 0 and p2->wselected < 10)
						{
							change_wall_place(x, y, &(p2->walls[p2->wselected]));
						}
						else if (y >= 60 and y < 60 + 2 * squaresize + margins)
						{
							int spx = which_wall(x, turn);
							if (spx < 0)
								return;
							if (p2->wselected != spx and p2->wselected >= 0 and p2->wselected < 10)
								p2->walls[p2->wselected].draw_wall();
							p2->wselected = spx;
							p2->walls[spx].set_focus();
						}
					}
				}
			}
		SDL_Rect temp;
		temp.x = 0;
		temp.y = 0;
		temp.w = SCRX;
		temp.h = SCRY;
		SDL_UpdateWindowSurfaceRects(screen, &temp, 1);
	}
};