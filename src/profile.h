/*
 *  Registry Profiler
 *  Copyright (c) 2022 Codiak
 */

#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>
#include <string>

#define PROF_FILE "ux0:data/PSREGPROF/profiles.txt"
#define DFLT_NAME "DFLT"
#define NO_PROF "-    -    -    -"
#define NO_ENTRY "-"
#define ON_ENTRY "_"

#define SAV_SIZE 12
#define REG_SIZE 27

#define REG1_ID  0
#define REG2_ID  1
#define REG3_ID  2
#define REG4_ID  3
#define REG5_ID  4

#define REG6_ID  5
#define REG7_ID  6
#define REG8_ID  7
#define REG9_ID  8
#define REG10_ID 9
#define REG11_ID 10
#define REG12_ID 11
#define REG13_ID 12

#define REG14_ID 13
#define REG15_ID 14
#define REG16_ID 15
#define REG17_ID 16
#define REG18_ID 17

#define REG19_ID 18
#define REG20_ID 19
#define REG21_ID 20
#define REG22_ID 21

#define REG23_ID 22
#define REG24_ID 23
#define REG25_ID 24
#define REG26_ID 25
#define REG27_ID 26

class Profile
{
	static std::string prof_set[SAV_SIZE];
	static int reg_set[REG_SIZE];
	
	static int prof_total;
public:
	static std::string GetProfName(int index);
	static int GetProfTotal();
	
	static bool GetProf(int index, std::string prof);
	static void SetProf(int index, std::string prof);
	static void SetDefaultProf(int index, std::string prof);
	
	static int GetReg(int index);
	static void SetReg(int index);
};

#endif