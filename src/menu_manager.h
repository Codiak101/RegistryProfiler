#ifndef _MENU_MANAGER_H
#define _MENU_MANAGER_H

#include <psp2/ctrl.h>
/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include <string>
#include <vita2d.h>
#include "menu.h"

#define MENU_SIZE 6
#define BUTT_HELD 20

class MenuManager
{
	Menu *menu[MENU_SIZE];
	Menu *currentMenu;
	
	int menu_index, menu_refresh;
	unsigned int pressed_button, held_button;
public:
	MenuManager();
	~MenuManager();
	
	void handleInput(SceCtrlData *ctrl_data, bool &app_exit);
	void handleInput(SceCtrlData *ctrl_data, bool &app_exit, int item_value);
	bool buttonPressed(unsigned int x, unsigned int button);
	void draw(vita2d_pgf *pgf);
	
	void addMenu(int index, Menu *menu, bool selected);
	void changeMenu(int index, Menu *menu);
	void firstMenu();
	void resetMenu();
	
	int getIndex();
	int getType();
	int getRefresh();
	
	void setItem(int index, std::string name, int is_active);
	int getItemVal();
	bool getItemActive();
};

#endif