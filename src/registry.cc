/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include <psp2/registrymgr.h>
#include "registry.h"

Registry::Registry()
{
	
}

Registry::Registry(std::string path, std::string code, std::string name, std::string description, int control, int min_value, int max_value, int snap_value)
{
	this->is_change = false;
	
	this->path = path;
	this->code = code;
	this->name = name;
	this->description = description;
	
	this->control = control;
	this->min_value = min_value;
	this->max_value = max_value;
	this->snap_value = snap_value;
}

//set the new registry value in the setting
void Registry::SetReg(int reg_value)
{
	const char *path = this->path.c_str();
	const char *code = this->code.c_str();
	
	if (reg_value > -1)
		sceRegMgrSetKeyInt(path, code, reg_value);
}

//get the registry name and value to display
//set the new registry value in the setting and display
std::string Registry::GetSetReg(int button_value)
{
	const char *path = this->path.c_str();
	const char *code = this->code.c_str();
	int value = 0;
	
	sceRegMgrGetKeyInt(path, code, &value);
	std::string output_name = this->name;
	
	if (this->control == CONTROL_BOX)
	{
		if (button_value != 0)
		{
			value = (value == 0 ? 1 : 0);
			sceRegMgrSetKeyInt(path, code, value);
		}
		
		if (value == 0)
			output_name += "    [    ]";
		else
			output_name += "    [X]";
	}
	else
	{
		switch (button_value)
		{
			case -1:
				value--;
				break;
			case 1:
				value++;
				break;
			case -2:
				value -= this->snap_value;
				break;
			case 2:
				value += this->snap_value;
				break;
			case -3:
				value = this->min_value;
				break;
			case 3:
				value = this->max_value;
				break;
			case 0:
			default:
				break;
		}
		
		if (button_value != 0)
		{
			value = (value < 0 ? 0 : value);
			sceRegMgrSetKeyInt(path, code, value);
		}
		
		output_name += "\n";
		output_name += std::to_string(min_value);
		output_name += "    >    ";
		output_name += std::to_string(value);
		output_name += std::to_string(max_value).size() > 9 ? "\n   " : "";
		output_name += "    <    ";
		output_name += std::to_string(max_value);
	}
	
	return output_name;
}

//get the registry name and value or just the value
std::string Registry::GetNameValue(bool name)
{
	std::string output_name;
	int value = 0;
	
	sceRegMgrGetKeyInt(this->path.c_str(), this->code.c_str(), &value);
	output_name = (name ? this->name + "    " : "");
	output_name += std::to_string(value);
	
	return output_name;
}

//get the registry description
std::string Registry::GetDesc()
{
	return this->description;
}

//get the registry control (tickbox or number)
int Registry::GetControl()
{
	return this->control;
}

//get whether the registry setting has changed in the session
bool Registry::GetChangeState()
{
	return this->is_change;
}

//set the registry setting as changed in the session
void Registry::SetChangeState(bool is_change)
{
	this->is_change = is_change;
}

//get whether the current device is a Vita or PSTV using an identifying registry setting
bool Registry::GetPSTVCheck()
{
	int pstv = 0;
	sceRegMgrGetKeyInt("/CONFIG/SHELL", "touch_emulation_help", &pstv);
	
	return (pstv == 1 ? true : false);
}