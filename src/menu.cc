/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include "menu.h"

Menu::Menu()
{
	
}

Menu::Menu(std::string name, float font_size, int x, int y, int type)
: name(name), font_size(font_size), x(x), y(y), background_colour(BLACK), current_menu_selection(0), total_menu_items(0)
{
	this->backgroundTexture = NULL;
	this->type = type;
	
	for (int i = 0; i < ITEM_SIZE; i++)
		this->menuItem[i] = NULL;
}

Menu::~Menu()
{
	for (int i = 0; i < ITEM_SIZE; i++)
		delete this->menuItem[i];
}

//handle the pad input
void Menu::handleInput(int prev_next, bool selected, int *button_value)
{
	if (selected && this->menuItem[this->current_menu_selection])
	{
		this->menuItem[this->current_menu_selection]->handleInput(button_value);
	}
	else if (prev_next != 0)
	{
		this->current_menu_selection += prev_next;
		
		if (this->current_menu_selection < 0)
			this->current_menu_selection = this->total_menu_items - 1;
		else if (this->current_menu_selection >= this->total_menu_items)
			this->current_menu_selection = 0;
	}
}

//draw the menu and the items
void Menu::draw(vita2d_pgf *pgf)
{
	if (!this->backgroundTexture)
		vita2d_set_clear_color(this->background_colour);
	else
		vita2d_draw_texture(this->backgroundTexture, 0, 0);
	
	if (this->font_size > 0)
		vita2d_pgf_draw_text(pgf, this->x, this->y, GREEN, this->font_size, this->name.c_str());
	
	for (int i = 0; i < this->total_menu_items; i++)
		this->menuItem[i]->draw(pgf, this->current_menu_selection == i);
}

//load the background image
void Menu::setBackground(std::string path)
{
	vita2d_free_texture(this->backgroundTexture);
	this->backgroundTexture = vita2d_load_PNG_file(path.c_str());
}

//load the background colour (default black)
void Menu::setBackground(int backgroundColour = BLACK)
{
	if (this->backgroundTexture)
	{
		vita2d_free_texture(this->backgroundTexture);
		this->backgroundTexture = NULL;
	}
	
	this->background_colour = backgroundColour;
}

//reset the selection to the first menu item
void Menu::resetMenu()
{
	this->current_menu_selection = 0;
}

//add the item to the menu
void Menu::addItem(MenuItem *item)
{
	this->menuItem[total_menu_items] = item;
	this->total_menu_items++;
}

//set the name and activation of the menu item
void Menu::setItem(int index, std::string name, int is_active)
{
	index = (index < 0 ? this->current_menu_selection : index);
	this->menuItem[index]->setNameActive(name, is_active);
}

//set the item active and inactive selection function
void Menu::setItemFnc(int index, void (*no_select_fnc)(int, int*), void (*on_select_fnc)(int, int*))
{
	this->menuItem[index]->setFunction(no_select_fnc, on_select_fnc);
}

//get the type of the menu
int Menu::getType()
{
	return this->type;
}

//get the name of the menu item
std::string Menu::getItemName(int index)
{
	index = (index < 0 ? this->current_menu_selection : index);
	return this->menuItem[index]->getName();
}

//get the index of the menu item
int Menu::getItemIndex()
{
	return this->current_menu_selection;
}

//get the count of the menu items
int Menu::getItemCount()
{
	return this->total_menu_items;
}

//get the function value of the menu item
int Menu::getItemVal()
{
	return this->menuItem[this->current_menu_selection]->getFncVal();
}

//get the menu item activation
bool Menu::getItemActive()
{
	return this->menuItem[this->current_menu_selection]->getActive();
}