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
bool isAltHeld = false;
#define KeyDown -32768
#define KeyUp 0

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
	ReadProcessMemory(hProcess, (BYTE*)(val.ClocalPlayer + m_iTeamNum), &team, sizeof(myTeam), NULL);
	return team;
}

void output()
{
	system("CLS");
	std::cout << "Game Found! Running Trainer... \n";
	std::cout << "--------------------------------------------------------- \n \n \n";
	if (bunnyHopStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "BHOP [F6] -> " << status << " <- \n \n";
	if (glowStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "WallHack [F7] -> " << status << " <- \n \n";
	if (brightnessStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "Brightness/Color Hack [F8] -> " << status << " <- \n \n";
	if (triggerbotStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "TriggerBot [F9] -> " << status << " <- \n \n";
	if (antiFlashStatus)
	{
		status = on;
	}
	else
	{
		status = off;
	}
	std::cout << "Anti-Flash [F11] -> " << status << " <- \n \n \n \n";
	std::cout << "EXIT [F4] \n";
}

void bunnyHop()
{
	{
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			bunnyHopStatus = !bunnyHopStatus;
			output();
		}
		if (!bunnyHopStatus)
		{
			return;
		}
		ReadProcessMemory(hProcess, (BYTE*)(val.ClocalPlayer + m_fFlags), &m_fFlagsValue, sizeof(m_fFlagsValue), NULL);
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
		ReadProcessMemory(hProcess, (BYTE*)(val.moduleBase + dwEntityList + i * 0x10), &entity, sizeof(entity), NULL);
		if (entity != NULL)
		{
			ReadProcessMemory(hProcess, (BYTE*)(entity + m_iTeamNum), &entTeam, sizeof(entTeam), NULL);
			if (myTeam == entTeam)
			{
				WriteProcessMemory(hProcess, (BYTE*)(entity + m_clrRender), &clrRenderTeam, sizeof(clrRenderTeam), NULL);
			}
			else
			{
				WriteProcessMemory(hProcess, (BYTE*)(entity + m_clrRender), &clrRenderEnemy, sizeof(clrRenderEnemy), NULL);
			}
		}
	}
}

void setBrightness()
{
	float brightness = 5.0f;

	int ptr;
	ReadProcessMemory(hProcess, (BYTE*)(val.engineModule + model_ambient_min), &ptr, sizeof(ptr), NULL);
	int xorptr = *(int*)&brightness ^ ptr;
	WriteProcessMemory(hProcess, (BYTE*)(val.engineModule + model_ambient_min), &xorptr, sizeof(xorptr), NULL);

	setColor();
}

void resetBrightness()
{
	float brightness = 0.0f;

	int ptr;
	ReadProcessMemory(hProcess, (BYTE*)(val.engineModule + model_ambient_min), &ptr, sizeof(ptr), NULL);
	int xorptr = *(int*)&brightness ^ ptr;
	WriteProcessMemory(hProcess, (BYTE*)(val.engineModule + model_ambient_min), &xorptr, sizeof(xorptr), NULL);

	setColor();
}

glowStruct setGlowColor(glowStruct Glow, uintptr_t entity, int health)
{
	ReadProcessMemory(hProcess, (BYTE*)(entity + m_bIsDefusing), &isDefusing, sizeof(isDefusing), NULL);
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
	ReadProcessMemory(hProcess, (BYTE*)(val.glowObject + (index * 0x38)), &TGlow, sizeof(TGlow), NULL);
	
	TGlow.blue = 1.0f;
	TGlow.alpha = 1.0f;
	TGlow.renderWhenOccluded = true;
	TGlow.renderWhenUnOccluded = false;
	WriteProcessMemory(hProcess, (BYTE*)(val.glowObject + (index * 0x38)), &TGlow, sizeof(TGlow), NULL);

}

void setEnemyGlow(uintptr_t entity, int index, int health)
{
	glowStruct EGlow;
	ReadProcessMemory(hProcess, (BYTE*)(val.glowObject + (index * 0x38)), &EGlow, sizeof(EGlow), NULL);

	EGlow = setGlowColor(EGlow, entity, health);
	WriteProcessMemory(hProcess, (BYTE*)(val.glowObject + (index * 0x38)), &EGlow, sizeof(EGlow), NULL);
}

void glow()
{
	ReadProcessMemory(hProcess, (BYTE*)(val.moduleBase + dwGlowObjectManager), &val.glowObject, sizeof(val.glowObject), NULL);
	bool dormant = false;
	for (short int i = 1; i < 16; i++)
	{
		ReadProcessMemory(hProcess, (BYTE*)(val.moduleBase + dwEntityList + i * 0x10), &entity, sizeof(entity), NULL);
		ReadProcessMemory(hProcess, (BYTE*)(entity + m_bDormant), &dormant, sizeof(dormant), NULL);
		if (entity != NULL && dormant == false)
		{
			ReadProcessMemory(hProcess, (BYTE*)(entity + m_iHealth), &hp, sizeof(hp), NULL);
			if (hp > 0)
			{
				ReadProcessMemory(hProcess, (BYTE*)(entity + m_iGlowIndex), &glowIndex, sizeof(glowIndex), NULL);
				ReadProcessMemory(hProcess, (BYTE*)(entity + m_iTeamNum), &entityTeam, sizeof(entityTeam), NULL);
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
	if (GetAsyncKeyState(VK_F11) & 1)
	{
		antiFlashStatus = !antiFlashStatus;
		output();
	}

	if (!antiFlashStatus)
	{
		return;
	}

	ReadProcessMemory(hProcess, (BYTE*)(val.ClocalPlayer + m_flFlashDuration), &flashDur, sizeof(flashDur), NULL);

	if (flashDur > 0)
	{
		flashDur = 0;
		WriteProcessMemory(hProcess, (BYTE*)(val.ClocalPlayer + m_flFlashDuration), &flashDur, sizeof(flashDur), NULL);
	}
}

void shoot()
{
	INPUT leftMouseShoot = { 0 };
	//press
	leftMouseShoot.type = INPUT_MOUSE;
	leftMouseShoot.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &leftMouseShoot, sizeof(INPUT));

	std::this_thread::sleep_for(std::chrono::milliseconds(30));

	// Release
	leftMouseShoot.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &leftMouseShoot, sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void triggerBot()
{
	if (GetAsyncKeyState(VK_MENU) == KeyDown && !isAltHeld)
	{
		isAltHeld = true;
		triggerbotStatus = true;
		output();
	}
	if (GetAsyncKeyState(VK_MENU) == KeyUp && isAltHeld)
	{
		isAltHeld = false;
		triggerbotStatus = false;
		output();
	}

	if (!triggerbotStatus)
	{
		return;
	}
	ReadProcessMemory(hProcess, (BYTE*)(val.ClocalPlayer + m_iCrosshairId), &crosshairID, sizeof(crosshairID), NULL);
	if (crosshairID != 0 && crosshairID < 32)
	{
		shoot();
	}
}

int main()
{
	// Get process ID
	DWORD procID = GetProcId(L"csgo.exe");
	if (procID == NULL)
	{
		std::cout << "Game not found \n";
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

		while (!GetAsyncKeyState(VK_F4))
		{
			// Get ClocalPlayer
			ReadProcessMemory(hProcess, (BYTE*)(val.moduleBase + dwLocalPlayer), &val.ClocalPlayer, sizeof(val.ClocalPlayer), NULL);

			if (!val.ClocalPlayer) continue;

			myTeam = getPlayerTeam();

			// Triggerbot
			triggerBot();

			// BHOP
			bunnyHop();

			// Glow
			if (GetAsyncKeyState(VK_F7) & 1)
			{
				glowStatus = !glowStatus;
				output();
			}

			if (glowStatus)
			{
				glow();
			}

			// Color/Brightness
			if (GetAsyncKeyState(VK_F8) & 1)
			{
				brightnessStatus = !brightnessStatus;
				if (brightnessStatus)
				{
					setBrightness();
				}
				else
				{
					resetBrightness();
				}
				output();
			}

			if (brightnessStatus)
			{
				setColor();
			}

			// Anti-Flash
			antiFlash();

			Sleep(1);
		}
	}
}

