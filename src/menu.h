/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#ifndef _MENU_H
#define _MENU_H

#include <string>
#include <vita2d.h>
#include "menu_item.h"

#define TYPE_TAB   1
#define TYPE_LIST  2
#define TYPE_GRID  3
#define TYPE_ENTRY 4
#define ITEM_SIZE  10

class Menu
{
	vita2d_texture *backgroundTexture;
	MenuItem *menuItem[ITEM_SIZE];
	
	std::string name;
	float font_size;
	int x, y, type;
	int background_colour;
	int current_menu_selection;
	int total_menu_items;
public:
	Menu();
	Menu(std::string name, float font_size, int x, int y, int type);
	~Menu();
	
	void handleInput(int prev_next, bool selected, int *button_value);
	void draw(vita2d_pgf *pgf);
	void setBackground(std::string path);
	void setBackground(int backgroundColour);
	
	void resetMenu();
	void addItem(MenuItem *item);
	void setItem(int index, std::string name, int is_active);
	void setItemFnc(int index, void (*no_select_fnc)(int, int*), void (*on_select_fnc)(int, int*));
	
	int getType();
	std::string getItemName(int index);
	int getItemIndex();
	int getItemCount();
	int getItemVal();
	bool getItemActive();
};

#endif