#pragma once
#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "common.h"
using namespace std;
using namespace SDL_setting;
class Textbar
{ // container of text

    SDL_Rect text_Rect;
    void draw_area()
    {
        grad_square(x - 20, y - 5, width, height, textbar_color_up, textbar_color_down);
        // grad_square(0,0,SCRX,40,textbar_color_up,textbar_color_down);horizontal header
    }

public:
    bool mouse_over;
    Textbar()
    {
        x = 20;
        y = 5;
        width = SCRX;
        height = 40;
        text_Rect.x = 20;
        text_Rect.y = 5;
        text_Rect.w = SCRX;
        text_Rect.h = 40;
        font = TTF_OpenFont("FreeSansBold.ttf", 22);
        show_text("Welcome to Quoridor");
    }
    Textbar(int posx, int posy, int w, int h, string str)
    {
        x = posx;
        y = posy;
        width = w;
        height = h;
        text_Rect.x = posx;
        text_Rect.y = posy;
        text_Rect.w = w;
        text_Rect.h = h;
        font = TTF_OpenFont("FreeSansBold.ttf", 22);
        show_text(str);
    }
    void show_text(string new_text)
    {
        draw_area();
        SDL_Surface *textSurface;
        // textSurface = TTF_RenderText_Solid(font, new_text.c_str(), textbar_textcolor);
        // SDL_Surface *sText;
        textSurface = TTF_RenderText_Blended(font, new_text.c_str(), textbar_textcolor);
        SDL_BlitSurface(textSurface, NULL, SDL_GetWindowSurface(screen), &text_Rect);
        SDL_FreeSurface(textSurface);
        SDL_UpdateWindowSurfaceRects(screen, &text_Rect, 1);
        // SDL_UpdateWindowSurfaceRects(screen, &text_Rect, 1);
        // SDL_RenderCopy(renderer, textTexture, NULL, &text_Rect);
        // SDL_RenderPresent(renderer);
    }

private:
    int width, height, x, y;
};

/*class Textbar
{
    SDL_Rect text_bar;
    void draw_textarea()
    {
        grad_square(0, 0, SCRX, 40, textbar_color_down, textbar_color_up);
    }

public:
    bool mouse_over;
    Textbar()
    {
        text_bar.x = 20;
        text_bar.y = 5;
        font = TTF_OpenFont("FreeSansBold.ttf", 22);
        if (font == NULL)
        {
            cout << "Error";
            exit(1);
        }
        change_text("Welcome to Corridor");
    }
    void change_text(std::string new_text)
    {
        draw_textarea();
        SDL_Surface *sText;
        sText = TTF_RenderText_Blended(font, new_text.c_str(), textbar_textcolor);
        SDL_BlitSurface(sText, NULL, SDL_GetWindowSurface(screen), &text_bar);
        SDL_FreeSurface(sText);

        SDL_UpdateWindowSurfaceRects(screen, &text_bar, 1);

        /*SDL_Surface *textSurface;
        textSurface = TTF_RenderText_Blended(font, new_text.c_str(), textbar_textcolor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_RenderCopy(renderer, textTexture, NULL, &text_bar);
        SDL_RenderPresent(renderer);*/
//}
//}
//;