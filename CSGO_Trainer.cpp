#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "proc.h"
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

struct variables
{
	uintptr_t ClocalPlayer;
	uintptr_t glowObject;
	uintptr_t engineModule;
	uintptr_t moduleBase;
} val;

struct glowStruct
{
	BYTE base[4];
	float red;
	float green;
	float blue;
	float alpha;
	BYTE buffer[16];
	bool renderWhenOccluded;
	bool renderWhenUnOccluded;
	bool fullBloom;
	BYTE buffer1;
	BYTE buffer2[4];
	int glowStyle;
} Glow;

#define F4_Key 0x73
#define F6_Key 0x75
#define F7_Key 0x76
#define F8_Key 0x77
#define F9_Key 0x78
#define F11_Key 0x7A

// Bhop
#define FL_ONGROUND 257
bool b_true = true;
bool b_false = false;
bool t_true = true;
bool t_false = false;
int m_fFlagsValue;

// Glow
int myTeam;
int entity;
int hp;
float teamBlue = 1;
float enemyRed = 1;
float enemyGreen = 1;
float enemyBlue = 1;
float alpha = 1.7f;
int glowIndex;
int entityTeam;
bool isDefusing = false;
bool glowTrue = true;
bool glowFalse = false;

std::vector<int> playerlist;

// Triggerbot
int crosshairID;

// Flash
int flashDur = 0;

// Status
bool bunnyHopStatus = false;
bool glowStatus = false;
bool triggerbotStatus = false;
bool brightnessStatus = false;
bool antiFlashStatus = false;
std::string on = "ON";
std::string off = "OFF";
std::string status = off;

HANDLE hProcess = 0;

int getPlayerTeam() {
	int team;
	ReadProcessMemory(hProcess, (PBYTE*)(val.ClocalPlayer + m_iTeamNum), &team, sizeof(myTeam), NULL);
	return team;
}

void JDAIWDIAJDAD()
{
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
}

void JDddawdwaIWDIdwadawdadawdAJDAD()
{
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
	std::cout << "duawhduahwduwahduwhadhwa";
}

void output()
{
	system("CLS");
	std::cout << "Game Found! Running Trainer..." << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl << std::endl << std::endl;
	if (bunnyHopStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "BHOP [F6] -> " << status << " <-" << std::endl << std::endl;
	if (glowStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "WallHack [F7] -> " << status << " <-" << std::endl << std::endl;
	if (brightnessStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "Brightness/Color Hack [F8] -> " << status << " <-" << std::endl << std::endl;
	if (triggerbotStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "TriggerBot [F9] -> " << status << " <-" << std::endl << std::endl;
	if (antiFlashStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "Anti-Flash [F11] -> " << status << " <-" << std::endl << std::endl << std::endl;
	std::cout << std::endl << std::endl << "EXIT [F4]" << std::endl;
}

void bunnyHop()
{
	{
		if (GetAsyncKeyState(F6_Key))
		{
			bunnyHopStatus = !bunnyHopStatus;
			output();
			Sleep(250);
		}
		if (!bunnyHopStatus)
		{
			return;
		}
		ReadProcessMemory(hProcess, (PBYTE*)(val.ClocalPlayer + m_fFlags), &m_fFlagsValue, sizeof(m_fFlagsValue), NULL);
		if (m_fFlagsValue == FL_ONGROUND)
		{
			WriteProcessMemory(hProcess, (BYTE*)(val.moduleBase + dwForceJump), &b_true, sizeof(b_true), NULL);
		}
		else if (m_fFlagsValue != FL_ONGROUND)
		{
			WriteProcessMemory(hProcess, (BYTE*)(val.moduleBase + dwForceJump), &b_false, sizeof(b_false), NULL);
		}
	}
}

struct ClrRender
{
	BYTE red, green, blue;
};
ClrRender clrRenderEnemy;
ClrRender clrRenderTeam;

std::vector<int> entityList()
{

	return playerlist;
}

void setBrightness()
{
	float brightness = 5.0f;

	int ptr;
	ReadProcessMemory(hProcess, (PBYTE*)(val.engineModule + model_ambient_min), &ptr, sizeof(ptr), NULL);
	int xorptr = *(int*)&brightness ^ ptr;
	WriteProcessMemory(hProcess, (PBYTE*)(val.engineModule + model_ambient_min), &xorptr, sizeof(xorptr), NULL);
}

void resetBrightness()
{
	float brightness = 0.0f;

	int ptr;
	ReadProcessMemory(hProcess, (PBYTE*)(val.engineModule + model_ambient_min), &ptr, sizeof(ptr), NULL);
	int xorptr = *(int*)&brightness ^ ptr;
	WriteProcessMemory(hProcess, (PBYTE*)(val.engineModule + model_ambient_min), &xorptr, sizeof(xorptr), NULL);
}

void setColor()
{
	int entTeam;
	if (brightnessStatus == true)
	{
		clrRenderEnemy.red = 255;
		clrRenderEnemy.green = 0;
		clrRenderEnemy.blue = 0;
		clrRenderTeam.red = 0;
		clrRenderTeam.green = 0;
		clrRenderTeam.blue = 255;
	}
	else
	{
		clrRenderEnemy.red = -1;
		clrRenderEnemy.green = -1;
		clrRenderEnemy.blue = -1;
		clrRenderTeam.red = -1;
		clrRenderTeam.green = -1;
		clrRenderTeam.blue = -1;
	}

	for (short int i = 1; i < 16; i++)
	{
		ReadProcessMemory(hProcess, (PBYTE*)(val.moduleBase + dwEntityList + i * 0x10), &entity, sizeof(entity), NULL);
		if (entity != NULL)
		{
			ReadProcessMemory(hProcess, (PBYTE*)(entity + m_iTeamNum), &entTeam, sizeof(entTeam), NULL);
			if (myTeam == entTeam)
			{
				WriteProcessMemory(hProcess, (PBYTE*)(entity + m_clrRender), &clrRenderTeam, sizeof(clrRenderTeam), NULL);
			}
			else 
			{
				WriteProcessMemory(hProcess, (PBYTE*)(entity + m_clrRender), &clrRenderEnemy, sizeof(clrRenderEnemy), NULL);
			}
		}
	}
}

glowStruct setGlowColor(glowStruct Glow, uintptr_t entity, int health)
{
	ReadProcessMemory(hProcess, (PBYTE*)(entity + m_bIsDefusing), &isDefusing, sizeof(isDefusing), NULL);
	if (isDefusing)
	{
		Glow.red = 1.0f;
		Glow.green = 1.0f;
		Glow.blue = 1.0f;
	}
	else
	{
		Glow.red = health * -0.01 + 1;
		Glow.green = health * 0.01;
	}
	Glow.alpha = 1.0f;
	Glow.renderWhenOccluded = true;
	Glow.renderWhenUnOccluded = false;
	return Glow;
}

void setTeamGlow(uintptr_t entity, int index)
{
	glowStruct TGlow;
	ReadProcessMemory(hProcess, (PBYTE*)(val.glowObject + (index * 0x38)), &TGlow, sizeof(TGlow), NULL);
	
	TGlow.blue = 1.0f;
	TGlow.alpha = 1.0f;
	TGlow.renderWhenOccluded = true;
	TGlow.renderWhenUnOccluded = false;
	WriteProcessMemory(hProcess, (PBYTE*)(val.glowObject + (index * 0x38)), &TGlow, sizeof(TGlow), NULL);

}

void setEnemyGlow(uintptr_t entity, int index, int health)
{
	glowStruct EGlow;
	ReadProcessMemory(hProcess, (PBYTE*)(val.glowObject + (index * 0x38)), &EGlow, sizeof(EGlow), NULL);

	EGlow = setGlowColor(EGlow, entity, health);
	WriteProcessMemory(hProcess, (PBYTE*)(val.glowObject + (index * 0x38)), &EGlow, sizeof(EGlow), NULL);
}

void glow()
{
	ReadProcessMemory(hProcess, (PBYTE*)(val.moduleBase + dwGlowObjectManager), &val.glowObject, sizeof(val.glowObject), NULL);
	bool dormant = false;
	for (short int i = 1; i < 16; i++)
	{
		ReadProcessMemory(hProcess, (PBYTE*)(val.moduleBase + dwEntityList + i * 0x10), &entity, sizeof(entity), NULL);
		ReadProcessMemory(hProcess, (PBYTE*)(entity + m_bDormant), &dormant, sizeof(dormant), NULL);
		if (entity != NULL && dormant == false)
		{
			ReadProcessMemory(hProcess, (PBYTE*)(entity + m_iHealth), &hp, sizeof(hp), NULL);
			if (hp > 0)
			{
				ReadProcessMemory(hProcess, (PBYTE*)(entity + m_iGlowIndex), &glowIndex, sizeof(glowIndex), NULL);
				ReadProcessMemory(hProcess, (PBYTE*)(entity + m_iTeamNum), &entityTeam, sizeof(entityTeam), NULL);
				if (myTeam == entityTeam)
				{
					// Friends
					setTeamGlow(entity, glowIndex);
				}
				else
				{
					// Enemies
					setEnemyGlow(entity, glowIndex, hp);
				}
			}
		}	
	}
}

void antiFlash()
{
	if (GetAsyncKeyState(F11_Key))
	{
		antiFlashStatus = !antiFlashStatus;
		output();
		Sleep(250);
	}

	if (!antiFlashStatus)
	{
		return;
	}

	ReadProcessMemory(hProcess, (PBYTE*)(val.ClocalPlayer + m_flFlashDuration), &flashDur, sizeof(flashDur), NULL);

	if (flashDur > 0)
	{
		flashDur = 0;
		WriteProcessMemory(hProcess, (PBYTE*)(val.ClocalPlayer + m_flFlashDuration), &flashDur, sizeof(flashDur), NULL);
	}
}

void triggerBot()
{
	if (GetAsyncKeyState(F9_Key))
	{
		triggerbotStatus = !triggerbotStatus;
		output();
		Sleep(250);
	}

	if (!triggerbotStatus)
	{
		return;
	}
	ReadProcessMemory(hProcess, (PBYTE*)(val.ClocalPlayer + m_iCrosshairId), &crosshairID, sizeof(crosshairID), NULL);
	if (crosshairID != 0 && crosshairID < 32)
	{
		WriteProcessMemory(hProcess, (PBYTE*)(val.moduleBase + dwForceAttack), &t_true, sizeof(t_true), NULL);
		WriteProcessMemory(hProcess, (PBYTE*)(val.moduleBase + dwForceAttack), &t_false, sizeof(t_false), NULL);
	}
}

int main()
{
	// Get process ID
	DWORD procID = GetProcId(L"csgo.exe");
	if (procID == NULL)
	{
		std::cout << "Game not found" << std::endl;
		Sleep(2000);
	}
	else
	{
		// Get Module and Engine base address
		val.moduleBase = GetModuleBaseAddress(procID, L"client_panorama.dll");
		val.engineModule = GetModuleBaseAddress(procID, L"engine.dll");

		// Get handle to the process
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

		// Output
		output();

		// Brightness
		setBrightness();

		while (!GetAsyncKeyState(F4_Key))
		{
			// Get ClocalPlayer
			ReadProcessMemory(hProcess, (PBYTE*)(val.moduleBase + dwLocalPlayer), &val.ClocalPlayer, sizeof(val.ClocalPlayer), NULL);
			myTeam = getPlayerTeam();

			// Triggerbot
			triggerBot();

			// BHOP
			bunnyHop();

			// Glow
			if (GetAsyncKeyState(F7_Key))
			{
				glowStatus = !glowStatus;
				output();
				Sleep(250);
			}

			if (glowStatus)
			{
				glow();
			}

			// Color/Brightness
			if (GetAsyncKeyState(F8_Key))
			{
				brightnessStatus = !brightnessStatus;
				if (brightnessStatus == true)
				{
					setBrightness();
				}
				else
				{
					resetBrightness();
				}
				output();
				Sleep(250);
			}
			setColor();

			// Anti-Flash
			antiFlash();

			Sleep(1);
		}
	}
}

