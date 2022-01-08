/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#include "profile.h"

std::string Profile::prof_set[SAV_SIZE];
int Profile::reg_set[REG_SIZE];
int Profile::prof_total = 0;

//get the selected saved profile name
std::string Profile::GetProfName(int index)
{
	int pos = prof_set[index].find(",");
	return (pos > -1 ? prof_set[index].substr(0, pos) : "");
}

//get the number of saved profiles
int Profile::GetProfTotal()
{
	return prof_total;
}

//get the current saved profiles from the file, else set the default profile in a new file
bool Profile::GetProf(int index, std::string prof)
{
	prof_total = 0;
	SceIoStat stat;
	
	if (sceIoGetstat(PROF_FILE, &stat) > -1)
	{
		char profs[stat.st_size];
		
		SceUID fd = sceIoOpen(PROF_FILE, SCE_O_RDONLY, 0777);
		sceIoRead(fd, profs, stat.st_size);
		sceIoClose(fd);
		
		std::string prof_set_new(profs);
		int pos_prof = -1;
		
		for (int i = 0; i < SAV_SIZE; i++)
		{
			if (i == 0 || pos_prof > -1)
			{
				prof_set_new = prof_set_new.substr(pos_prof + 1, prof_set_new.size());
				pos_prof = prof_set_new.find("|");
				
				std::string prof_old = prof_set_new.substr(0, pos_prof);
				int pos_reg = prof_old.find(",");
				
				if (pos_prof > -1 && pos_reg > -1)
				{
					prof_set[i] = prof_old;
					prof_total++;
				}
				else
					prof_set[i].clear();
			}
			else
				prof_set[i].clear();
		}
		
		return true;
	}
	else
	{
		SetDefaultProf(index, prof);
		return false;
	}
}

//save the current profile name and registry values into the selected save slot
//delete the saved profile in the selected save slot
void Profile::SetProf(int index, std::string prof)
{
	std::string prof_set_new;
	int pos = prof.find(NO_PROF);
	
	if (pos < 0)
	{
		if (prof_set[index].empty())
			prof_total++;
		
		prof_set[index] = prof;
	}
	else
	{
		if (!prof_set[index].empty())
			prof_total--;
		
		prof_set[index].clear();
	}
	
	for (int i = 0; i < SAV_SIZE; i++)
	{
		pos = prof_set[i].find(",");
		prof_set_new += (pos > -1 ? prof_set[i] : NO_ENTRY) + "|";
	}
	
	sceIoRemove(PROF_FILE);
	SceUID fd = sceIoOpen(PROF_FILE, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	sceIoWrite(fd, prof_set_new.c_str(), prof_set_new.size());
	sceIoClose(fd);
}

//save the current registry values as the default profile if no save file
void Profile::SetDefaultProf(int index, std::string prof)
{
	sceIoMkdir("ux0:data/PSREGPROF", 0777);
	prof_set[index] = prof;
	prof_total = 1;
	
	SceUID fd = sceIoOpen(PROF_FILE, SCE_O_WRONLY | SCE_O_CREAT, 0777);
	sceIoWrite(fd, prof_set[index].c_str(), prof_set[index].size());
	sceIoClose(fd);
}

//get the selected registry value of the current profile
int Profile::GetReg(int index)
{
	return reg_set[index];
}

//set all the selected registry values of the current profile
void Profile::SetReg(int index)
{
	if (index > -1)
	{
		int pos = prof_set[index].find(",");
		
		if (pos > -1)
		{
			std::string prof_set_new = prof_set[index];
			
			for (int i = 0; i < REG_SIZE; i++)
			{
				prof_set_new = prof_set_new.substr(pos + 1, prof_set_new.size());
				pos = prof_set_new.find(",");
				
				reg_set[i] = std::stoi(prof_set_new.substr(0, pos));
			}
		}
	}
	else
	{
		for (int i = 0; i < REG_SIZE; i++)
			reg_set[i] = -1;
	}
}