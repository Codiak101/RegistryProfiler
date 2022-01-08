/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#ifndef _MENU_ITEM_H
#define _MENU_ITEM_H

#include <string>
#include <vita2d.h>

#define RGBA8(r, g, b, a) ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))
#define RED   RGBA8(255,0,0,255)
#define GREEN RGBA8(0,255,0,255)
#define BLUE  RGBA8(0,0,255,255)
#define BLACK RGBA8(0,0,0,255)
#define GREY  RGBA8(125,125,125,255)
#define WHITE RGBA8(255,255,255,255)

class MenuItem
{
	std::string name;
	int x, y, draw_colour, fnc_value;
	float font_size;
	bool is_active;
	
	void (*no_select_fnc)(int, int*);
	void (*on_select_fnc)(int, int*);
public:
	MenuItem(std::string name, float font_size, int x, int y,
			 void (*no_select_fnc)(int, int*), void (*on_select_fnc)(int, int*), int fnc_value, bool is_active);
	~MenuItem();
	
	bool handleInput(int *button_value);
	void draw(vita2d_pgf *pgf, bool selected);
	
	void setNameActive(std::string name, int is_active);
	void setFunction(void (*no_select_fnc)(int, int*), void (*on_select_fnc)(int, int*));
	std::string getName();
	int getFncVal();
	bool getActive();
};

#endif