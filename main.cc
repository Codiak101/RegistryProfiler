/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/kernel/processmgr.h>
#include <string>
#include <vita2d.h>
#include "src/menu_manager.h"
#include "src/menu.h"
#include "src/menu_item.h"
#include "src/registry.h"
#include "src/profile.h"

#define TITLE "Registry Profiler by Codiak    |    "
#define PROFILE  0
#define SERVICE  1
#define DISPLAY  2
#define SOUND	 3
#define CONTROL  4
#define POWER	 5
#define SAVE_F	 -1
#define SAVE_C	 -2
#define LOAD	 -3
#define LOAD_C	 -4
#define DELETE	 -5
#define DELETE_C -6
#define CTRLS	 -7
#define REBOOT	 -8
#define EXIT	 -9

#define LFONT_SIZE 1.4f
#define SFONT_SIZE 1.2f

MenuManager *manager = NULL;
Menu *menu = NULL;
Menu *prof = NULL;
Menu *save = NULL;
Menu *load = NULL;
Menu *kill = NULL;
Registry *reg = NULL;
std::string save_log;
std::string save_values;
int save_slot = 0;
int reg_change = 0;
bool app_exit = false;

void loadMenu();
void loadReg();
void refreshReg();
void loadItem(int menu_index, int item_index, int &reg_ctrl, vita2d_pgf *menu_font,
			  vita2d_texture *menu_in, vita2d_texture *menu_save, vita2d_texture *menu_boot);
void profileReg(vita2d_pgf *menu_font, bool is_load, bool is_change, bool is_save);
void registryProf(vita2d_pgf *menu_font, void (*on_select_fnc)(int, int*));

void updateReg(int fnc_value, int *button_value);
void saveProf(int fnc_value, int *button_value);
void manageProf(int fnc_value, int *button_value);
void deviceReboot(int, int*)
{ scePowerRequestColdReset(); }
void appExit(int, int*)
{ app_exit = true; }
void itemEmpty(int, int*) {}

int main()
{
	vita2d_init();
	vita2d_texture *menu_bg = vita2d_load_PNG_file("app0:data/menu_bg.png");
	vita2d_texture *menu_in = vita2d_load_PNG_file("app0:data/menu_in.png");
	vita2d_texture *menu_save = vita2d_load_PNG_file("app0:data/menu_save.png");
	vita2d_texture *menu_boot = vita2d_load_PNG_file("app0:data/menu_boot.png");
	vita2d_pgf *menu_font = vita2d_load_custom_pgf("app0:data/menu_font.pgf");
	
	int reg_ctrl = 0;
	Profile::SetReg(-1);
	SceCtrlData pad;
	
	manager = new MenuManager();
	loadMenu();
	loadReg();
	
	while(1)
	{
		sceCtrlPeekBufferPositive(0, &pad, 1);
		manager->handleInput(&pad, app_exit, reg_ctrl);
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		vita2d_draw_texture(menu_bg, 0, 0);
		
		loadItem(manager->getIndex(), manager->getItemVal(), reg_ctrl, menu_font, menu_in, menu_save, menu_boot);
		refreshReg();
		manager->draw(menu_font);
		
		vita2d_end_drawing();
		vita2d_swap_buffers();
		
		if (app_exit)
			break;
	}
	
	vita2d_fini();
	vita2d_free_pgf(menu_font);
	vita2d_free_texture(menu_boot);
	vita2d_free_texture(menu_in);
	vita2d_free_texture(menu_save);
	vita2d_free_texture(menu_bg);
	
	sceKernelExitProcess(0);
	delete[] reg;
	delete[] kill;
	delete[] load;
	delete[] save;
	delete[] prof;
	delete[] menu;
	delete manager;
	
	return 0;
}

//load the menus with data to be used by the app
void loadMenu()
{
	bool isPSTV = Registry::GetPSTVCheck();
	
	menu = new Menu[MENU_SIZE];
	menu[0] = {"PROFILE", LFONT_SIZE, 30, 65, TYPE_LIST};
	Menu *currentMenu = &menu[0];
	currentMenu->addItem(new MenuItem("SAVE[F]", LFONT_SIZE, 30, 125, &itemEmpty, &saveProf, SAVE_F, true));
	currentMenu->addItem(new MenuItem("SAVE[C]", LFONT_SIZE, 30, 185, &itemEmpty, &saveProf, SAVE_C, true));
	currentMenu->addItem(new MenuItem("LOAD", LFONT_SIZE, 30, 245, &itemEmpty, &manageProf, LOAD, true));
	currentMenu->addItem(new MenuItem("DELETE", LFONT_SIZE, 30, 305, &itemEmpty, &manageProf, DELETE, true));
	currentMenu->addItem(new MenuItem("CONTROLS", LFONT_SIZE, 30, 365, &itemEmpty, &itemEmpty, CTRLS, true));
	currentMenu->addItem(new MenuItem("REBOOT", LFONT_SIZE, 30, 425, &itemEmpty, &deviceReboot, REBOOT, false));
	currentMenu->addItem(new MenuItem("EXIT", LFONT_SIZE, 30, 485, &itemEmpty, &appExit, EXIT, true));
	manager->addMenu(PROFILE, currentMenu, true);
	
	menu[1] = {"SERVICE", LFONT_SIZE, 189, 65, TYPE_LIST};
	currentMenu = &menu[1];
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 100, &itemEmpty, &updateReg, REG1_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 150, &itemEmpty, &updateReg, REG2_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 200, &itemEmpty, &updateReg, REG3_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 250, &itemEmpty, &updateReg, REG4_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 300, &itemEmpty, &updateReg, REG5_ID, true));
	manager->addMenu(SERVICE, currentMenu, false);
	
	menu[2] = {"DISPLAY", LFONT_SIZE, 348, 65, TYPE_LIST};
	currentMenu = &menu[2];
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 100, &itemEmpty, &updateReg, REG6_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 150, &itemEmpty, &updateReg, REG7_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 200, &itemEmpty, &updateReg, REG8_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 250, &itemEmpty, &updateReg, REG9_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 300, &itemEmpty, &updateReg, REG10_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 350, &itemEmpty, &updateReg, REG11_ID, isPSTV ? true : false));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 400, &itemEmpty, &updateReg, REG12_ID, isPSTV ? true : false));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 450, &itemEmpty, &updateReg, REG13_ID, isPSTV ? true : false));
	manager->addMenu(DISPLAY, currentMenu, false);
	
	menu[3] = {"SOUND", LFONT_SIZE, 507, 65, TYPE_LIST};
	currentMenu = &menu[3];
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 100, &itemEmpty, &updateReg, REG14_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 150, &itemEmpty, &updateReg, REG15_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 200, &itemEmpty, &updateReg, REG16_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 250, &itemEmpty, &updateReg, REG17_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 300, &itemEmpty, &updateReg, REG18_ID, true));
	manager->addMenu(SOUND, currentMenu, false);
	
	menu[4] = {"CONTROL", LFONT_SIZE, 666, 65, TYPE_LIST};
	currentMenu = &menu[4];
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 100, &itemEmpty, &updateReg, REG19_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 150, &itemEmpty, &updateReg, REG20_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 200, &itemEmpty, &updateReg, REG21_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 250, &itemEmpty, &updateReg, REG22_ID, true));
	manager->addMenu(CONTROL, currentMenu, false);
	
	menu[5] = {"POWER", LFONT_SIZE, 825, 65, TYPE_LIST};
	currentMenu = &menu[5];
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 100, &itemEmpty, &updateReg, REG23_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 150, &itemEmpty, &updateReg, REG24_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 200, &itemEmpty, &updateReg, REG25_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 250, &itemEmpty, &updateReg, REG26_ID, true));
	currentMenu->addItem(new MenuItem(NO_ENTRY, SFONT_SIZE, 5, 300, &itemEmpty, &updateReg, REG27_ID, true));
	manager->addMenu(POWER, currentMenu, false);
	
	prof = new Menu("", 0, 0, 0, TYPE_GRID);
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 348, 150, &itemEmpty, &itemEmpty, 27, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 598, 150, &itemEmpty, &itemEmpty, 28, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 848, 150, &itemEmpty, &itemEmpty, 29, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 348, 250, &itemEmpty, &itemEmpty, 30, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 598, 250, &itemEmpty, &itemEmpty, 31, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 848, 250, &itemEmpty, &itemEmpty, 32, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 348, 350, &itemEmpty, &itemEmpty, 33, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 598, 350, &itemEmpty, &itemEmpty, 34, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 848, 350, &itemEmpty, &itemEmpty, 35, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 348, 450, &itemEmpty, &itemEmpty, 36, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 598, 450, &itemEmpty, &itemEmpty, 37, true));
	prof->addItem(new MenuItem(NO_PROF, LFONT_SIZE, 848, 450, &itemEmpty, &itemEmpty, 38, true));
	
	save = new Menu("", 0, 0, 0, TYPE_ENTRY);
	save->addItem(new MenuItem(ON_ENTRY, LFONT_SIZE, 475, 300, &itemEmpty, &saveProf, 39, true));
	save->addItem(new MenuItem(ON_ENTRY, LFONT_SIZE, 575, 300, &itemEmpty, &saveProf, 40, true));
	save->addItem(new MenuItem(ON_ENTRY, LFONT_SIZE, 675, 300, &itemEmpty, &saveProf, 41, true));
	save->addItem(new MenuItem(ON_ENTRY, LFONT_SIZE, 775, 300, &itemEmpty, &saveProf, 42, true));
	
	load = new Menu("", 0, 0, 0, TYPE_ENTRY);
	load->addItem(new MenuItem("", SFONT_SIZE, 348, 100, &itemEmpty, &manageProf, LOAD_C, true));
	
	kill = new Menu("", 0, 0, 0, TYPE_ENTRY);
	kill->addItem(new MenuItem("", SFONT_SIZE, 348, 100, &itemEmpty, &manageProf, DELETE_C, true));
}

//load the registry settings with data to be used by the app
void loadReg()
{
	reg = new Registry[REG_SIZE];
	reg[REG1_ID] = {"/CONFIG/NET", "wifi_flag", "Wi-Fi", REG1_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG2_ID] = {"/CONFIG/BT", "bt_enable", "Bluetooth", REG2_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG3_ID] = {"/CONFIG/LOCATION", "enable_service", "Location Data", REG3_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG4_ID] = {"/CONFIG/SHELL", "notification", "Notification Alerts", REG4_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG5_ID] = {"/CONFIG/NP2/AUTOUPDATE", "system", "Auto-Download Sys Upd", REG5_DESC, CONTROL_BOX, 0, 1, 1};
	
	reg[REG6_ID] = {"/CONFIG/DISPLAY", "brightness", "Brightness", REG6_DESC, CONTROL_NUM, 21, 65536, 1000};
	reg[REG7_ID] = {"/CONFIG/DISPLAY", "color_space_mode", "Colour Space", REG7_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG8_ID] = {"/CONFIG/ACCESSIBILITY", "contrast", "High Contrast", REG8_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG9_ID] = {"/CONFIG/ACCESSIBILITY", "large_text", "Larger Text", REG9_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG10_ID] = {"/CONFIG/ACCESSIBILITY", "zoom", "Zoom", REG10_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG11_ID] = {"/CONFIG/DISPLAY", "rgb_range_mode", "RGB Range", REG11_DESC, CONTROL_NUM, 0, 2, 1};
	reg[REG12_ID] = {"/CONFIG/DISPLAY", "hdmi_resolution_mode", "Resolution", REG12_DESC, CONTROL_NUM, 0, 3, 1};
	reg[REG13_ID] = {"/CONFIG/DISPLAY", "hdmi_out_scaling_ratio", "Display Area", REG13_DESC, CONTROL_NUM, 0, 1000000000, 1000000};
	
	reg[REG14_ID] = {"/CONFIG/SOUND", "main_volume", "Main Volume", REG14_DESC, CONTROL_NUM, 0, 30, 5};
	reg[REG15_ID] = {"/CONFIG/SOUND", "speaker_mute", "Auto-Mute Speaker", REG15_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG16_ID] = {"/CONFIG/SHELL", "mic_mute", "Toggle Microphone", REG16_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG17_ID] = {"/CONFIG/SHELL", "bgm", "System Music", REG17_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG18_ID] = {"/CONFIG/SOUND", "avls", "AVLS", REG18_DESC, CONTROL_BOX, 0, 1, 1};
	
	reg[REG19_ID] = {"/CONFIG/SYSTEM", "key_pad", "Control with Buttons", REG19_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG20_ID] = {"/CONFIG/SYSTEM/", "controller_vibration", "Controller Vibration", REG20_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG21_ID] = {"/CONFIG/SYSTEM", "button_assign", "X/O Button Assign", REG21_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG22_ID] = {"/CONFIG/ACCESSIBILITY", "keyremap_enable", "Custom Button Assign", REG22_DESC, CONTROL_BOX, 0, 1, 1};
	
	reg[REG23_ID] = {"/CONFIG/POWER_SAVING", "suspend_interval", "Auto Standby", REG23_DESC, CONTROL_NUM, 60, 1800, 60};
	reg[REG24_ID] = {"/CONFIG/DISPLAY", "dimming_interval", "Dimming Interval", REG24_DESC, CONTROL_NUM, 60, 1800, 60};
	reg[REG25_ID] = {"/CONFIG/POWER_SAVING", "controller_off_interval", "Auto-Disable Controllers", REG25_DESC, CONTROL_NUM, 60, 1800, 60};
	reg[REG26_ID] = {"/CONFIG/USB", "usb_charge_enable", "USB Power Supply", REG26_DESC, CONTROL_BOX, 0, 1, 1};
	reg[REG27_ID] = {"/CONFIG/NET", "wifi_power_save", "Wi-Fi Power Save", REG27_DESC, CONTROL_BOX, 0, 1, 1};
}

//refresh the registry settings on the currently selected menu to update any values
//or reset to the first menu and clear cache data
void refreshReg()
{
	int refresh = manager->getRefresh();
	
	if (refresh == 1)
	{
		switch (manager->getIndex())
		{
			case PROFILE:
				manager->setItem(1, "SAVE[C]", reg_change == 1 ? 1 : -1);
				manager->setItem(2, "LOAD", Profile::GetProfTotal() > 0 ? 1 : -1);
				manager->setItem(3, "DELETE", Profile::GetProfTotal() > 1 ? 1 : -1);
				manager->setItem(5, "REBOOT", reg_change != 0 ? 1 : -1);
				break;
			case SERVICE:
				manager->setItem(0, reg[REG1_ID].GetSetReg(0), 0);
				manager->setItem(1, reg[REG2_ID].GetSetReg(0), 0);
				manager->setItem(2, reg[REG3_ID].GetSetReg(0), 0);
				manager->setItem(3, reg[REG4_ID].GetSetReg(0), 0);
				manager->setItem(4, reg[REG5_ID].GetSetReg(0), 0);
				break;
			case DISPLAY:
				manager->setItem(0, reg[REG6_ID].GetSetReg(0), 0);
				manager->setItem(1, reg[REG7_ID].GetSetReg(0), 0);
				manager->setItem(2, reg[REG8_ID].GetSetReg(0), 0);
				manager->setItem(3, reg[REG9_ID].GetSetReg(0), 0);
				manager->setItem(4, reg[REG10_ID].GetSetReg(0), 0);
				manager->setItem(5, reg[REG11_ID].GetSetReg(0), 0);
				manager->setItem(6, reg[REG12_ID].GetSetReg(0), 0);
				manager->setItem(7, reg[REG13_ID].GetSetReg(0), 0);
				break;
			case SOUND:
				manager->setItem(0, reg[REG14_ID].GetSetReg(0), 0);
				manager->setItem(1, reg[REG15_ID].GetSetReg(0), 0);
				manager->setItem(2, reg[REG16_ID].GetSetReg(0), 0);
				manager->setItem(3, reg[REG17_ID].GetSetReg(0), 0);
				manager->setItem(4, reg[REG18_ID].GetSetReg(0), 0);
				break;
			case CONTROL:
				manager->setItem(0, reg[REG19_ID].GetSetReg(0), 0);
				manager->setItem(1, reg[REG20_ID].GetSetReg(0), 0);
				manager->setItem(2, reg[REG21_ID].GetSetReg(0), 0);
				manager->setItem(3, reg[REG22_ID].GetSetReg(0), 0);
				break;
			case POWER:
				manager->setItem(0, reg[REG23_ID].GetSetReg(0), 0);
				manager->setItem(1, reg[REG24_ID].GetSetReg(0), 0);
				manager->setItem(2, reg[REG25_ID].GetSetReg(0), 0);
				manager->setItem(3, reg[REG26_ID].GetSetReg(0), 0);
				manager->setItem(4, reg[REG27_ID].GetSetReg(0), 0);
				break;
			default:
				break;
		}
	}
	else if (refresh == 2)
	{
		for (int i = 0; i < REG_SIZE; i++)
			reg[i].SetChangeState(false);
		
		reg_change = 0;
		Profile::SetReg(-1);
		
		save_slot = 0;
		save_values.clear();
		save_log = "[RESET]";
	}
}

//load any saved profiles, names/logs and info relevant to the currently selected menu item
void loadItem(int menu_index, int item_index, int &reg_ctrl, vita2d_pgf *menu_font,
			  vita2d_texture *menu_in, vita2d_texture *menu_save, vita2d_texture *menu_boot)
{
	if (save_log.empty())
	{
		profileReg(NULL, false, false, true);
		save_log = DFLT_NAME;
		
		if (!Profile::GetProf(0, save_log + "," + save_values))
			save_log = "[SAVED] Profile:  " + save_log;
		else
			save_log = " ";
	}
	
	vita2d_pgf_draw_text(menu_font, 5, 25, WHITE, LFONT_SIZE, (TITLE + save_log).c_str());
	vita2d_pgf_draw_text(menu_font, 30, 65, menu_index < 0 ? GREEN : WHITE, LFONT_SIZE, "PROFILE");
	vita2d_pgf_draw_text(menu_font, 189, 65, WHITE, LFONT_SIZE, "SERVICE");
	vita2d_pgf_draw_text(menu_font, 348, 65, WHITE, LFONT_SIZE, "DISPLAY");
	vita2d_pgf_draw_text(menu_font, 507, 65, WHITE, LFONT_SIZE, "SOUND");
	vita2d_pgf_draw_text(menu_font, 666, 65, WHITE, LFONT_SIZE, "CONTROL");
	vita2d_pgf_draw_text(menu_font, 825, 65, WHITE, LFONT_SIZE, "POWER");
	
	if (item_index < REG1_ID || item_index > REG27_ID)
	{
		reg_ctrl = 0;
		vita2d_pgf_draw_text(menu_font, 30, 125, (menu_index == SAVE_F ? GREEN : GREY), LFONT_SIZE, "SAVE[F]");
		vita2d_pgf_draw_text(menu_font, 30, 185, (menu_index == SAVE_C ? GREEN : GREY), LFONT_SIZE, "SAVE[C]");
		vita2d_pgf_draw_text(menu_font, 30, 245, (menu_index == LOAD || menu_index == LOAD_C ? GREEN : GREY), LFONT_SIZE, "LOAD");
		vita2d_pgf_draw_text(menu_font, 30, 305, (menu_index == DELETE || menu_index == DELETE_C ? GREEN : GREY), LFONT_SIZE, "DELETE");
		vita2d_pgf_draw_text(menu_font, 30, 365, GREY, LFONT_SIZE, "CONTROLS");
		vita2d_pgf_draw_text(menu_font, 30, 425, GREY, LFONT_SIZE, "REBOOT");
		vita2d_pgf_draw_text(menu_font, 30, 485, GREY, LFONT_SIZE, "EXIT");
	}
	
	if (item_index == SAVE_F)
	{
		vita2d_pgf_draw_text(menu_font, 348, 100, WHITE, SFONT_SIZE, "Save all current registry settings into a full profile?");
		profileReg(menu_font, false, false, false);
		registryProf(NULL, &saveProf);
	}
	else if (item_index == SAVE_C)
	{
		vita2d_pgf_draw_text(menu_font, 348, 100, WHITE, SFONT_SIZE, "Save changed registry settings into a partial profile?");
		profileReg(menu_font, false, true, false);
		registryProf(NULL, &saveProf);
	}
	else if (item_index == LOAD)
	{
		vita2d_pgf_draw_text(menu_font, 348, 100, WHITE, SFONT_SIZE, "Load one of the profiles below?");
		registryProf(menu_font, &manageProf);
	}
	else if (item_index == DELETE)
	{
		vita2d_pgf_draw_text(menu_font, 348, 100, WHITE, SFONT_SIZE, "Delete one of the profiles below?");
		registryProf(menu_font, &manageProf);
	}
	else if (item_index == LOAD_C || item_index == DELETE_C)
	{
		profileReg(menu_font, true, false, false);
	}
	else if (item_index == CTRLS)
	{
		vita2d_draw_texture(menu_in, 320, 75);
	}
	else if (item_index == REBOOT)
	{
		vita2d_draw_texture(menu_boot, 320, 75);
		vita2d_pgf_draw_text(menu_font, 400, 500, WHITE, SFONT_SIZE, "NOTE: Must have at least one setting updated");
	}
	else if (item_index > -1 && item_index < REG_SIZE)
	{
		reg_ctrl = reg[item_index].GetControl() == CONTROL_NUM ? 1 : 0;
		vita2d_pgf_draw_text(menu_font, 348, 100, WHITE, SFONT_SIZE, reg[item_index].GetDesc().c_str());
		
		if (item_index == REG6_ID)
			vita2d_pgf_draw_text(menu_font, 364, 360, RED, SFONT_SIZE, "[WARNING]");
	}
	else if (item_index >= REG_SIZE)
	{
		if (item_index < (REG_SIZE + SAV_SIZE))
		{
			std::string message;
			switch (menu_index)
			{
				case SAVE_F:
				case SAVE_C:
					message = "Save into one of the slots below?";
					break;
				case LOAD:
				case LOAD_C:
					message = "Load one of the profiles below?";
					break;
				case DELETE:
				case DELETE_C:
					message = "Delete one of the profiles below?";
					break;
				default:
					break;
			}
			
			vita2d_pgf_draw_text(menu_font, 348, 100, GREEN, SFONT_SIZE, message.c_str());
		}
		else if (item_index >= (REG_SIZE + SAV_SIZE))
		{
			vita2d_draw_texture(menu_save, 320, 75);
			vita2d_pgf_draw_text(menu_font, 425, 100, GREEN, SFONT_SIZE, "Please enter a 4 character name for the profile:");
		}
	}
	else if (item_index == EXIT)
	{
		vita2d_pgf_draw_text(menu_font, 350, 150, GREEN, LFONT_SIZE, "CREDITS");
		vita2d_pgf_draw_text(menu_font, 350, 200, GREY, LFONT_SIZE, "[GITHUB]");
		vita2d_pgf_draw_text(menu_font, 500, 200, GREEN, LFONT_SIZE, "xerpi");
		vita2d_pgf_draw_text(menu_font, 700, 200, WHITE, LFONT_SIZE, "libvita2d");
		
		vita2d_pgf_draw_text(menu_font, 350, 250, GREY, LFONT_SIZE, "[GITHUB]");
		vita2d_pgf_draw_text(menu_font, 500, 250, GREEN, LFONT_SIZE, "kprinssu");
		vita2d_pgf_draw_text(menu_font, 700, 250, WHITE, LFONT_SIZE, "vitamenulib");
		
		vita2d_pgf_draw_text(menu_font, 350, 300, GREY, LFONT_SIZE, "[GITHUB]");
		vita2d_pgf_draw_text(menu_font, 500, 300, GREEN, LFONT_SIZE, "devnoname120");
		vita2d_pgf_draw_text(menu_font, 700, 300, WHITE, LFONT_SIZE, "RegistryEditorMOD");
		
		vita2d_pgf_draw_text(menu_font, 350, 350, GREY, LFONT_SIZE, "[GITHUB]");
		vita2d_pgf_draw_text(menu_font, 500, 350, GREEN, LFONT_SIZE, "CallmeBK");
		vita2d_pgf_draw_text(menu_font, 700, 350, WHITE, LFONT_SIZE, "VitaBatteryAlarm");
		
		vita2d_pgf_draw_text(menu_font, 350, 400, GREY, LFONT_SIZE, "[TESTER]");
		vita2d_pgf_draw_text(menu_font, 500, 400, GREEN, LFONT_SIZE, "Gruffy D");
		vita2d_pgf_draw_text(menu_font, 700, 400, WHITE, LFONT_SIZE, "VITA & PSTV");
	}
}

//create the profile view that shows the values on all the registry settings and save for later
void profileReg(vita2d_pgf *menu_font, bool is_load, bool is_change, bool is_save)
{
	std::string entry;
	int pos_x;
	int pos_y;
	
	if (menu_font != NULL)
	{
		vita2d_pgf_draw_text(menu_font, 348, 150, WHITE, SFONT_SIZE, "SERVICE");
		vita2d_pgf_draw_text(menu_font, 348, 225, WHITE, SFONT_SIZE, "DISPLAY");
		vita2d_pgf_draw_text(menu_font, 348, 325, WHITE, SFONT_SIZE, "SOUND");
		vita2d_pgf_draw_text(menu_font, 348, 400, WHITE, SFONT_SIZE, "CONTROL");
		vita2d_pgf_draw_text(menu_font, 348, 475, WHITE, SFONT_SIZE, "POWER");
	}
	
	for (int i = 0; i < REG_SIZE; i++)
	{
		if (is_load)
		{
			entry = std::to_string(Profile::GetReg(i));
			entry = (entry == "-1" ? NO_ENTRY : entry);
		}
		else if (!is_change || reg[i].GetChangeState())
			entry = reg[i].GetNameValue(false);
		else
			entry = NO_ENTRY;
		
		if (menu_font != NULL)
		{
			switch (i)
			{
				case REG1_ID:
					pos_x = 348;
					pos_y = 175;
					break;
				case REG6_ID:
					pos_x = 348;
					pos_y = 250;
					break;
				case REG10_ID:
					pos_x = 348;
					pos_y = 275;
					break;
				case REG14_ID:
					pos_x = 348;
					pos_y = 350;
					break;
				case REG19_ID:
					pos_x = 348;
					pos_y = 425;
					break;
				case REG23_ID:
					pos_x = 348;
					pos_y = 500;
					break;
				default:
					break;
			}
			
			vita2d_pgf_draw_text(menu_font, pos_x, pos_y, WHITE, SFONT_SIZE, entry.c_str());
			pos_x += 100;
		}
		
		if (is_save)
		{
			if (entry == NO_ENTRY)
				save_values += "-1";
			else
				save_values += entry;
			
			save_values += (i < REG27_ID ? "," : "");
		}
	}
}

//create the profile selection with existing profiles and empty slots
void registryProf(vita2d_pgf *menu_font, void (*on_select_fnc)(int, int*))
{
	int pos_x = 348;
	int pos_y = 150;
	
	for (int i = 0; i < SAV_SIZE; i++)
	{
		std::string prof_name = Profile::GetProfName(i);
		void (*new_select_fnc)(int, int*);
		int is_active;
		
		if (!prof_name.empty() || on_select_fnc == &saveProf)
		{
			prof_name = (!prof_name.empty() ? prof_name : NO_PROF);
			new_select_fnc = on_select_fnc;
			is_active = 1;
		}
		else
		{
			prof_name = NO_PROF;
			new_select_fnc = &itemEmpty;
			is_active = -1;
		}
		
		if (menu_font != NULL)
		{
			if (i > 0 && i % TYPE_GRID == 0)
			{
				pos_x = 348;
				pos_y += 100;
			}
			
			vita2d_pgf_draw_text(menu_font, pos_x, pos_y, GREY, LFONT_SIZE, prof_name.c_str());
			pos_x += 250;
		}
		
		prof->setItem(i, prof_name, is_active);
		prof->setItemFnc(i, &itemEmpty, new_select_fnc);
	}
}

//update the selected registry setting with the new value
void updateReg(int fnc_value, int *button_value)
{
	std::string old_value = reg[fnc_value].GetNameValue(true);
	manager->setItem(-1, reg[fnc_value].GetSetReg(*button_value), 0);
	std::string new_value = reg[fnc_value].GetNameValue(true);
	
	reg[fnc_value].SetChangeState(true);
	reg_change = 1;
	
	save_log = (new_value != old_value ? new_value : save_log);
	save_values.clear();
}

//save the currently selected settings into the selected slot with the entered name
void saveProf(int fnc_value, int *button_value)
{
	if (button_value == NULL)
	{
		switch (manager->getType())
		{
			case TYPE_ENTRY:
				manager->changeMenu(manager->getIndex(), prof);
				break;
			case TYPE_GRID:
			default:
				manager->firstMenu();
				break;
		}
	}
	else if (fnc_value == SAVE_F || fnc_value == SAVE_C)
	{
		prof->resetMenu();
		manager->changeMenu(fnc_value, prof);
	}
	else if (fnc_value >= REG_SIZE && fnc_value < (REG_SIZE + SAV_SIZE))
	{
		for (int i = 0; i < save->getItemCount(); i++)
		{
			if (prof->getItemName(-1) != NO_PROF)
			{
				std::string new_item(1, prof->getItemName(-1)[i]);
				save->setItem(i, new_item, 1);
			}
			else
				save->setItem(i, ON_ENTRY, 1);
		}
		
		save->resetMenu();
		manager->changeMenu(manager->getIndex(), save);
		save_slot = fnc_value - REG_SIZE;
	}
	else if (*button_value != 0)
	{
		char old_item = save->getItemName(-1)[0];
		
		switch (old_item)
		{
			case '_':
				old_item = *button_value > 0 ? 'A' : '9';
				break;
			case 'A':
				old_item = *button_value > 0 ? 'B' : '9';
				break;
			case 'Z':
				old_item = *button_value > 0 ? 'a' : 'Y';
				break;
			case 'a':
				old_item = *button_value > 0 ? 'b' : 'Z';
				break;
			case 'z':
				old_item = *button_value > 0 ? '0' : 'y';
				break;
			case '0':
				old_item = *button_value > 0 ? '1' : 'z';
				break;
			case '9':
				old_item = *button_value > 0 ? 'A' : '8';
				break;
			default:
				old_item += *button_value > 0 ? 1 : -1;
				break;
		}
		
		std::string new_item(1, old_item);
		save->setItem(-1, new_item, 1);
		manager->changeMenu(manager->getIndex(), save);
	}
	else if (*button_value == 0)
	{
		std::string prof_name;
		
		for (int i = 0; i < save->getItemCount(); i++)
			prof_name += save->getItemName(i);
		
		if (prof_name.find(ON_ENTRY) == std::string::npos)
		{
			save_values.clear();
			
			if (manager->getIndex() == SAVE_F)
				profileReg(NULL, false, false, true);
			else if (manager->getIndex() == SAVE_C)
				profileReg(NULL, false, true, true);
			
			Profile::SetProf(save_slot, prof_name + "," + save_values);
			save_log = "[SAVED] Profile:  " + prof_name;
			
			for (int i = 0; i < REG_SIZE; i++)
				reg[i].SetChangeState(false);
			
			reg_change = (reg_change == 1 ? -1 : reg_change);
			manager->firstMenu();
		}
	}
}

//load or delete the currently selected profile
void manageProf(int fnc_value, int *button_value)
{
	if (button_value == NULL)
	{
		switch (manager->getIndex())
		{
			case LOAD_C:
				manager->changeMenu(LOAD, prof);
				break;
			case DELETE_C:
				manager->changeMenu(DELETE, prof);
				break;
			case LOAD:
			case DELETE:
			default:
				manager->firstMenu();
				break;
		}
	}
	else if (fnc_value == LOAD || fnc_value == DELETE)
	{
		prof->resetMenu();
		manager->changeMenu(fnc_value, prof);
	}
	else if (fnc_value >= REG_SIZE && fnc_value < (REG_SIZE + SAV_SIZE))
	{
		save_slot = fnc_value - REG_SIZE;
		Profile::SetReg(save_slot);
		std::string message = " profile " + Profile::GetProfName(save_slot) + " of registry values below?    X [YES] / O [NO]";
		
		if (manager->getIndex() == LOAD)
		{
			load->setItem(LOAD_C, "Load" + message, 1);
			manager->changeMenu(LOAD_C, load);
		}
		else if (manager->getIndex() == DELETE)
		{
			kill->setItem(DELETE_C, "Delete" + message, 1);
			manager->changeMenu(DELETE_C, kill);
		}
	}
	else if (*button_value == 0)
	{
		if (fnc_value == LOAD_C)
		{
			for (int i = 0; i < REG_SIZE; i++)
			{
				int reg_value = Profile::GetReg(i);
				
				if (reg_value > -1)
					reg[i].SetReg(reg_value);
				
				reg[i].SetChangeState(false);
			}
			
			reg_change = -1;
			save_log = "[LOADED] Profile:  " + Profile::GetProfName(save_slot);
			save_values.clear();
		}
		else if (fnc_value == DELETE_C)
		{
			Profile::SetReg(-1);
			save_log = "[DELETED] Profile:  " + Profile::GetProfName(save_slot);
			Profile::SetProf(save_slot, NO_PROF);
		}
		
		manager->firstMenu();
	}
}