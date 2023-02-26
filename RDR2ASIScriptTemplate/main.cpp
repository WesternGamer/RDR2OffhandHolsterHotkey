#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include "natives.h"
#include "enums.h"
#include "types.h"
#include "Keyboard.h"
#include "Scripthook.h"
#include "Pools.h"
#include "Config.h"

using namespace Keyboard;
using namespace Pools;
using namespace std;
namespace fs = std::filesystem;

namespace RDR2ASIScriptTemplate 
{
	DWORD Key = VK_F11;

	/// <summary>
    /// Called every game update.
    /// </summary>
	void Update()
	{
		if (IsKeyJustUp(Key, false)) {
			PED::REMOVE_SHOP_ITEM_FROM_PED_BY_CATEGORY(PLAYER::PLAYER_PED_ID(), 0xB6B6122D, 0, false);
			PED::_UPDATE_PED_VARIATION(PLAYER::PLAYER_PED_ID(), false, true, true, true, false);
		}
	}

	/// <summary>
	/// This function is triggered when you interact with the game for the first time after the world loads.
	/// </summary>
	void OnWorldLoad()
	{
		srand(GetTickCount());

		char* configName = _strdup("RDR2OffhandHolsterHotkey.ini");
		char* topic = _strdup("RDR2OffhandHolsterHotkeySettings");
		char* toggleKey = _strdup("toggleKey");
		char* keyValue = _strdup("0x7A");

		if (fs::exists(configName) == false) {
			Config::saveinis(configName, topic, toggleKey, keyValue);
		}

		Config::readinis(configName, topic, toggleKey, keyValue);

		std::stringstream ss;
		ss << std::hex << keyValue;
		ss >> Key;

		while (true)
		{
			Update();
			WAIT(0);
		}
	}

	
}

/// <summary>
	/// Main entry point of ASI script.
	/// </summary>
	/// <param name="hInstance">Handle to DLL module.</param>
	/// <param name="reason">Reason for calling function.</param>
	/// <param name="lpReserved">Reserved.</param>
	/// <returns>True</returns>
BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		scriptRegister(hInstance, RDR2ASIScriptTemplate::OnWorldLoad);
		keyboardHandlerRegister(Keyboard::OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(Keyboard::OnKeyboardMessage);
		break;
	}
	return true;
}





