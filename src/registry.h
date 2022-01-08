/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#ifndef __REG_H__
#define __REG_H__

#include <string>

#define CONTROL_BOX 1
#define CONTROL_NUM 2

#define REG1_DESC "[VITA] Settings > Network > Wi-Fi Settings\n\n[VITA] PS Button Quick Menu > Wi-Fi\n\n[PSTV] Settings > Network > Internet Connection Settings\n\n/CONFIG/NET/wifi_flag\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG2_DESC "Settings > Devices > Bluetooth Devices\n\n[VITA] PS Button Quick Menu > Bluetooth Feature\n\n/CONFIG/BT/bt_enable\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG3_DESC "[VITA] Settings > Location Data > Use Location Data\n\n/CONFIG/LOCATION/enable_service\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG4_DESC "Settings > Notifications > Enable Notification Alerts\n\n/CONFIG/SHELL/notification\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = INSTANT"
#define REG5_DESC "Settings > System > Auto-Start Settings\n\n/CONFIG/NP2/AUTOUPDATE/system\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"

#define REG6_DESC "[VITA] Settings > Sound & Display > Brightness\n\n[VITA] PS Button Quick Menu > Brightness\n\n/CONFIG/DISPLAY/brightness\n\n\n> DEVICE = VITA & PSTV\n\n> Can go outside official min/max values\n\n> Value can't go below 0\n\n> [WARNING]  0 = Screen OFF (not when docked)\n\n> X/O Snap value = 1000 (hold for max/min value)\n\n>> APPLY = Enter Official Setting"
#define REG7_DESC "No Official Setting\n\n/CONFIG/DISPLAY/color_space_mode\n\n\n> DEVICE = VITA[SLIM] & PSTV\n\n> Improves colour saturation/contrast\n\n>> APPLY = REBOOT"
#define REG8_DESC "Settings > Accessibility > High Contrast\n\n/CONFIG/ACCESSIBILITY/contrast\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = INSTANT/RELAUNCH"
#define REG9_DESC "Settings > Accessibility > Larger Text\n\n/CONFIG/ACCESSIBILITY/large_text\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = INSTANT/RELAUNCH"
#define REG10_DESC "Settings > Accessibility > Zoom\n\n/CONFIG/ACCESSIBILITY/zoom\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = INSTANT"
#define REG11_DESC "[PSTV] Settings > Sound & Display > RGB Range\n\n/CONFIG/DISPLAY/rgb_range_mode\n\n\n> DEVICE = PSTV\n\n> Can go above official max value\n\n> Value can't go below 0\n\n> 0 = Auto    |    1 = Limited    |    2 = Full\n\n> X/O Snap value = 1 (hold for max/min value)\n\n>> APPLY = REBOOT"
#define REG12_DESC "[PSTV] Settings > Sound & Display > Resolution\n\n/CONFIG/DISPLAY/hdmi_resolution_mode\n\n\n> DEVICE = PSTV\n\n> Can go above official max value\n\n> Value can't go below 0\n\n> 0 = Auto    |    1 = 1080i    |    2 = 720p    |    3 = 480p\n\n> X/O Snap value = 1 (hold for max/min value)\n\n>> APPLY = REBOOT"
#define REG13_DESC "[PSTV] Settings > Sound & Display > Display Area Settings\n\n/CONFIG/DISPLAY/hdmi_out_scaling_ratio\n\n\n> DEVICE = PSTV\n\n> Can go above official max value\n\n> Value can't go below 0\n\n> X/O Snap value = 1000000 (hold for max/min value)\n\n>> APPLY = REBOOT"

#define REG14_DESC "[VITA] -VOL+ Buttons\n\n[PSTV] PS Button Quick Menu > Master Volume\n\n/CONFIG/SOUND/main_volume\n\n\n> DEVICE = VITA & PSTV\n\n> Can go above official max value\n\n> Value can't go below 0\n\n> X/O Snap value = 5 (hold for max/min value)\n\n>> APPLY = REBOOT"
#define REG15_DESC "[VITA] Settings > Sound & Display > Mute Automatically\n\n/CONFIG/SOUND/speaker_mute\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG16_DESC "PS Button Quick Menu > Disable Microphone\n\n/CONFIG/SHELL/mic_mute\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG17_DESC "Settings > Sound & Display > System Music\n\n/CONFIG/SHELL/bgm\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG18_DESC "[VITA] Settings > Sound & Display > AVLS\n\n/CONFIG/SOUND/avls\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"

#define REG19_DESC "[VITA] Settings > System > Control with Buttons on This System\n\n/CONFIG/SYSTEM/key_pad\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = RELAUNCH/REBOOT"
#define REG20_DESC "[PSTV] PS Button Quick Menu > Enable Vibration\n\n/CONFIG/SYSTEM/controller_vibration\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG21_DESC "Settings > HENkaku Settings > O Button Behaviour\n\n/CONFIG/SYSTEM/button_assign\n\n\n> DEVICE = VITA & PSTV\n\n> Enter is O when OFF and X when ON\n\n>> APPLY = RELAUNCH/REBOOT"
#define REG22_DESC "Settings > Accessibility > Button Assignments\n\n/CONFIG/ACCESSIBILITY/keyremap_enable\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = INSTANT"

#define REG23_DESC "Settings > Power Save Settings > Enter Standby Mode Auto\n\n/CONFIG/POWER_SAVING/suspend_interval\n\n\n> DEVICE = VITA & PSTV\n\n> Can go outside official min/max values\n\n> Value can't go below 0\n\n> 0 = OFF\n\n> 60 = 1 min\n\n> X/O Snap value = 60 (hold for max/min value)\n\n>> APPLY = REBOOT"
#define REG24_DESC "No Official Setting\n\n/CONFIG/DISPLAY/dimming_interval\n\n\n> DEVICE = VITA & PSTV\n\n> Can go outside official min/max values\n\n> Value can't go below 0\n\n> 0 = OFF [REQ suspend_interval = 0]\n\n> 60 = 1 min\n\n> X/O Snap value = 60 (hold for max/min value)\n\n>> APPLY = REBOOT"
#define REG25_DESC "[PSTV] Settings > Power Save > Turn Off Controllers Auto\n\n/CONFIG/POWER_SAVING/controller_off_interval\n\n\n> DEVICE = VITA & PSTV\n\n> Can go outside official min/max values\n\n> Value can't go below 0\n\n> 0 = OFF\n\n> 60 = 1 min\n\n> X/O Snap value = 60 (hold for max/min value)\n\n>> APPLY = REBOOT"
#define REG26_DESC "[VITA] Settings > System > USB Power Supply\n\n/CONFIG/USB/usb_charge_enable\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"
#define REG27_DESC "Settings > Power Save Settings > Use Wi-Fi in Power Save Mode\n\n/CONFIG/NET/wifi_power_save\n\n\n> DEVICE = VITA & PSTV\n\n>> APPLY = REBOOT"

class Registry
{
	std::string path,
				code,
				name,
				description;
	
	int control,
		min_value,
		max_value,
		snap_value;
	
	bool is_change;
public:
	Registry();
	Registry(std::string path, std::string code, std::string name, std::string description, int control, int min_value, int max_value, int snap_value);
	
	void SetReg(int reg_value);
	std::string GetSetReg(int button_value);
	std::string GetNameValue(bool name);
	std::string GetDesc();
	int GetControl();
	bool GetChangeState();
	void SetChangeState(bool is_change);
	static bool GetPSTVCheck();
};

#endif