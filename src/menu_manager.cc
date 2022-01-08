/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include "menu_manager.h"

MenuManager::MenuManager()
{
	this->menu_index = 0;
	this->menu_refresh = 1;
	
	this->pressed_button = 0;
	this->held_button = 0;
	
	for (int i = 0; i < MENU_SIZE; i++)
		this->menu[i] = NULL;
}

MenuManager::~MenuManager()
{
	for (int i = 0; i < MENU_SIZE; i++)
		delete this->menu[i];
}

//handle the pad input to the profile menus
void MenuManager::handleInput(SceCtrlData *ctrl_data, bool &app_exit)
{
	if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_LTRIGGER))
	{
		this->changeMenu(MENU_SIZE - 1, this->menu[MENU_SIZE - 1]);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_RTRIGGER))
	{
		this->changeMenu(1, this->menu[1]);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_UP))
	{
		if (this->currentMenu->getType() == TYPE_GRID)
		{
			int button_value = 0;
			this->currentMenu->handleInput(-TYPE_GRID, false, &button_value);
			this->pressed_button = ctrl_data->buttons;
		}
		else if (this->currentMenu->getType() == TYPE_ENTRY)
		{
			int button_value = -1;
			this->held_button++;
			
			if (this->held_button == 1 || this->held_button > BUTT_HELD)
				this->currentMenu->handleInput(0, true, &button_value);
		}
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_DOWN))
	{
		if (this->currentMenu->getType() == TYPE_GRID)
		{
			int button_value = 0;
			this->currentMenu->handleInput(TYPE_GRID, false, &button_value);
			this->pressed_button = ctrl_data->buttons;
		}
		else if (this->currentMenu->getType() == TYPE_ENTRY)
		{
			int button_value = 1;
			this->held_button++;
			
			if (this->held_button == 1 || this->held_button > BUTT_HELD)
				this->currentMenu->handleInput(0, true, &button_value);
		}
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_LEFT))
	{
		int button_value = 0;
		this->currentMenu->handleInput(-1, false, &button_value);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_RIGHT))
	{
		int button_value = 0;
		this->currentMenu->handleInput(1, false, &button_value);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_CIRCLE))
	{
		if (this->getItemActive())
			this->currentMenu->handleInput(0, true, NULL);
		else
			this->firstMenu();
		
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_CROSS))
	{
		int button_value = 0;
		this->currentMenu->handleInput(0, true, &button_value);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_SQUARE))
	{
		this->firstMenu();
		this->resetMenu();
		this->held_button++;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_TRIANGLE))
	{
		app_exit = true;
		this->pressed_button = ctrl_data->buttons;
	}
	else
	{
		this->held_button = 0;
		this->pressed_button = ctrl_data->buttons;
	}
}

//handle the pad input to the tab menus
void MenuManager::handleInput(SceCtrlData *ctrl_data, bool &app_exit, int item_value)
{
	if (this->menu_index < 0)
	{
		this->handleInput(ctrl_data, app_exit);
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_LTRIGGER))
	{
		if ((this->menu_index - 1) < 0)
			this->menu_index = MENU_SIZE - 1;
		else
			this->menu_index--;
		
		this->changeMenu(this->menu_index, this->menu[this->menu_index]);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_RTRIGGER))
	{
		if ((this->menu_index + 1) > (MENU_SIZE - 1))
			this->menu_index = 0;
		else
			this->menu_index++;
		
		this->changeMenu(this->menu_index, this->menu[this->menu_index]);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->currentMenu->getType() == TYPE_LIST && this->buttonPressed(ctrl_data->buttons, SCE_CTRL_UP))
	{
		int button_value = 0;
		this->currentMenu->handleInput(-1, false, &button_value);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->currentMenu->getType() == TYPE_LIST && this->buttonPressed(ctrl_data->buttons, SCE_CTRL_DOWN))
	{
		int button_value = 0;
		this->currentMenu->handleInput(1, false, &button_value);
		this->pressed_button = ctrl_data->buttons;
	}
	else if (this->currentMenu->getType() == TYPE_LIST && this->buttonPressed(ctrl_data->buttons, SCE_CTRL_LEFT) &&
			 item_value == 1)
	{
		int button_value = -1;
		this->held_button++;
		
		if (this->held_button == 1 || this->held_button > BUTT_HELD)
			this->currentMenu->handleInput(0, true, &button_value);
	}
	else if (this->currentMenu->getType() == TYPE_LIST && this->buttonPressed(ctrl_data->buttons, SCE_CTRL_RIGHT) &&
			 item_value == 1)
	{
		int button_value = 1;
		this->held_button++;
		
		if (this->held_button == 1 || this->held_button > BUTT_HELD)
			this->currentMenu->handleInput(0, true, &button_value);
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_CIRCLE) && this->getItemVal() > -1)
	{
		if (item_value == 1)
		{
			this->held_button++;
			if (this->held_button == 1)
			{
				int button_value = -2;
				this->currentMenu->handleInput(0, true, &button_value);
			}
			else if (this->held_button == BUTT_HELD)
			{
				int button_value = -3;
				this->currentMenu->handleInput(0, true, &button_value);
			}
		}
		else
		{
			int button_value = -2;
			this->currentMenu->handleInput(0, true, &button_value);
			this->pressed_button = ctrl_data->buttons;
		}
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_CROSS))
	{
		if (item_value == 1)
		{
			this->held_button++;
			if (this->held_button == 1)
			{
				int button_value = 2;
				this->currentMenu->handleInput(0, true, &button_value);
			}
			else if (this->held_button == BUTT_HELD)
			{
				int button_value = 3;
				this->currentMenu->handleInput(0, true, &button_value);
			}
		}
		else
		{
			int button_value = 2;
			this->currentMenu->handleInput(0, true, &button_value);
			this->pressed_button = ctrl_data->buttons;
		}
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_SQUARE))
	{
		if (this->held_button == 0)
		{
			this->menu_refresh = 1;
			this->resetMenu();
			this->held_button++;
		}
		else
		{
			this->held_button++;
			if (this->held_button == BUTT_HELD)
				this->menu_refresh = 2;
			else if (this->held_button == BUTT_HELD + 1)
			{
				this->menu_refresh = 1;
				if (this->menu_index > 0)
				{
					this->firstMenu();
					this->resetMenu();
				}
			}
		}
	}
	else if (this->buttonPressed(ctrl_data->buttons, SCE_CTRL_TRIANGLE))
	{
		app_exit = true;
		this->pressed_button = ctrl_data->buttons;
	}
	else
	{
		this->held_button = 0;
		this->pressed_button = ctrl_data->buttons;
	}
}

//check if the button was pressed
bool MenuManager::buttonPressed(unsigned int x, unsigned int button)
{
	return (((this->pressed_button & button) == 0) && ((x & button) == button));
}

//draw the current menu
void MenuManager::draw(vita2d_pgf *pgf)
{
	this->currentMenu->draw(pgf);
}

//add the menu to the manager
void MenuManager::addMenu(int index, Menu *menu, bool selected) 
{
	this->menu[index] = menu;
	
	if (selected)
		this->changeMenu(index, menu);
}

//change the current menu
void MenuManager::changeMenu(int index, Menu *menu)
{
	this->menu_index = index;
	this->currentMenu = menu;
	this->menu_refresh = 1;
}

//change current menu to first menu
void MenuManager::firstMenu()
{
	this->changeMenu(0, this->menu[0]);
}

//reset current menu to first menu item
void MenuManager::resetMenu()
{
	this->currentMenu->resetMenu();
}

//get the current menu index
int MenuManager::getIndex()
{
	return this->menu_index;
}

//get the current menu type
int MenuManager::getType()
{
	return this->currentMenu->getType();
}

//get and reset the current menu refresh
int MenuManager::getRefresh()
{
	int refresh = this->menu_refresh;
	this->menu_refresh = 0;
	
	return refresh;
}

//set item of the current menu
void MenuManager::setItem(int index, std::string name, int is_active)
{
	this->currentMenu->setItem(index, name, is_active);
}

//get the current menu item value
int MenuManager::getItemVal()
{
	return this->currentMenu->getItemVal();
}

//get the current menu item activation
bool MenuManager::getItemActive()
{
	return this->currentMenu->getItemActive();
}