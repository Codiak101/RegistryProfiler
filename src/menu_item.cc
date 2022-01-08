/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include <vita2d.h>
#include "menu_item.h"

MenuItem::MenuItem(std::string name, float font_size, int x, int y,
				   void (*no_select_fnc)(int, int*), void (*on_select_fnc)(int, int*), int fnc_value, bool is_active)
{
	this->name = name;
	this->font_size = font_size;
	this->x = x;
	this->y = y;
	this->draw_colour = WHITE;
	
	this->no_select_fnc = no_select_fnc;
	this->on_select_fnc = on_select_fnc;
	this->fnc_value = fnc_value;
	this->is_active = is_active;
}

MenuItem::~MenuItem()
{
	
}

//trigger selection function
bool MenuItem::handleInput(int *button_value)
{
	if (this->is_active)
		(*this->on_select_fnc)(this->fnc_value, button_value);
	else
		(*this->no_select_fnc)(this->fnc_value, button_value);
	
	return true;
}

//draw the menu item
void MenuItem::draw(vita2d_pgf *pgf, bool selected) 
{
	if (this->is_active)
		this->draw_colour = selected ? GREEN : WHITE;
	else
		this->draw_colour = selected ? RED : GREY;
	
	vita2d_pgf_draw_text(pgf, this->x, this->y, this->draw_colour, this->font_size, this->name.c_str());
}

//set the name and activation
void MenuItem::setNameActive(std::string name, int is_active)
{
	this->name = name;
	
	if (is_active != 0)
		this->is_active = (is_active < 0 ? false : true);
}

//set the active and inactive selection function
void MenuItem::setFunction(void (*no_select_fnc)(int, int*), void (*on_select_fnc)(int, int*))
{
	this->no_select_fnc = no_select_fnc;
	this->on_select_fnc = on_select_fnc;
}

//get the name
std::string MenuItem::getName()
{
	return this->name;
}

//get the function value
int MenuItem::getFncVal()
{
	return this->fnc_value;
}

//get the item activation
bool MenuItem::getActive()
{
	return this->is_active;
}