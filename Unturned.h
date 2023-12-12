#pragma once
#include "DMA.h"
#include "graphics.h"
#include "XorString.h"

struct _Camera
{
	Vector3 right, up, forward, translation;
	Vector2 fov, screen;
}Camera;

Vector3 WorldToScreen(Vector3 WorldPos)
{
	Vector3 localize = WorldPos - Camera.translation;
	return Vector3(
		(Camera.screen.x / 2) * (1 + localize.dot(Camera.right) / Camera.fov.x / -localize.dot(Camera.forward)),
		(Camera.screen.y / 2) * (1 - localize.dot(Camera.up) / Camera.fov.y / -localize.dot(Camera.forward)),
		localize.dot(Camera.forward)
	);
}

#include "UserInterface.h"
#include "Offsets.h"
#include "Aimbot.h"
#include "Player.h"
#include "Vehicle.h"
#include "ModeConfig.h"

class Unturned
{
private:
	UserInterface* UI;
	Player* player;
	ModeConfig* configData;
	Vehicle* vehicle;

	uint64_t pCamera;

	void UpdateCamera()
	{
		uint8_t buffer[0x1000] = { 0 };
		uint32_t base = 0x4E8;
		read(pCamera, (uint64_t)buffer, 0x1000);
		Camera.right = *(Vector3*)&buffer[base + 0x00];
		Camera.up = *(Vector3*)&buffer[base + 0x10];
		Camera.forward = *(Vector3*)&buffer[base + 0x20];
		Camera.translation = *(Vector3*)&buffer[base + 0x30];
		Camera.fov.x = *(float*)&buffer[base - 0x40];
		Camera.fov.y = *(float*)&buffer[base - 0x34];
	}

	void UpdateConfigData()
	{

		if (UI->CompassEnabled() != configData->GetCompass())
			configData->SetCompass(UI->CompassEnabled());
		if (UI->GpsEnabled() != configData->GetSatellite())
		{
			configData->SetSatellite(UI->GpsEnabled());
			configData->SetChart(UI->GpsEnabled());
		}


		if (UI->exitTimer() != configData->GetTimer_Exit())
			configData->SetTimer_Exit(UI->exitTimer() * 30.f);
	}

public:

	Unturned()
	{
		SelectProcessA(XorString("Unturned.exe"));
		uint64_t UnityPlayer = ModuleBaseA(XorString("UnityPlayer.dll"));
		Camera.screen = Vector2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		uint64_t UnityBase = read<uint64_t>({ UnityPlayer + GameBase, 0xB0, 0x60 });
		uint64_t pModeConfig = read<uint64_t>(UnityBase + ConfigBase);
		uint64_t pSteamPlayer = read<uint64_t>(UnityBase + PlayerBase);
		uint64_t pInteractableVehicle = read<uint64_t>(UnityBase + VehicleBase);
		pCamera = read<uint64_t>({ UnityPlayer + CameraBase, 0x1A0, 0x48, 0xB0 });

		draw = new Overlay();
		UI = new UserInterface();
		player = new Player(pSteamPlayer);
		configData = new ModeConfig(read<uint64_t>(pModeConfig + ModeConfigData._Gameplay.self()));
		vehicle = new Vehicle(pInteractableVehicle);

		while (player->GetSalvageTime() < 0.f) { player->Update(); }

		UI->CompassEnabled(configData->GetCompass());
		UI->GpsEnabled(configData->GetSatellite());
		UI->exitTimer(configData->GetTimer_Exit() / 30.f);
		UI->salvageTime(player->GetSalvageTime() / 10.f);
		UI->enableSalvage(player->GetShouldSalvage());
		UI->isAimbotEnabled(true);
		UI->renderDistance(0.3334f);//1000 meters
		UI->aimbotFov(0.6f);// 300 pixels
		UI->isAimDotEnabled(true);
		UI->aimTarget(0);// head
		UI->weaponSpread(0.75f);
		UI->weaponRecoil(0.6f);
		UI->range(0.615385f);// 8 meters used in placeable range
		UI->offset(0.515f);// 0.016 meters
		UI->missChance(0.03f);
		UI->aimbotSmoothing(0.96f);
	}

	void MakeMeBetter()
	{
		while (!GetAsyncKeyState(VK_END)) 
		{ 
			draw->Begin();
			player->Update();
			vehicle->Update();

			player->EnableAimbot(UI->isAimbotEnabled());
			player->AimbotTarget(UI->aimTarget());
			player->AimMissChance(UI->missChance());
			player->Smoothing(UI->aimbotSmoothing());
			player->FOV(UI->aimbotFov() * 500.f);
			player->Spread(UI->weaponSpread());
			player->Recoil(UI->weaponRecoil());
			player->RenderDist(UI->renderDistance() * 3000.f);
			player->EnableAimdot(UI->isAimDotEnabled());
			if (player->desyncFreeBuild)
				UI->SetFreeBuild(player->GetFreeBuild());
			player->Range(UI->range());
			player->Offset(UI->offset());
			player->Pitch(UI->pitch());
			player->Yaw(UI->yaw());
			player->Roll(UI->roll());
			player->EnableSalvage(UI->enableSalvage());
			player->ExitTimer(UI->exitTimer() * 30.f);
			player->SalvageTime(UI->salvageTime() * 10.f);
			vehicle->SetRenderDistance(UI->renderDistance() * 3000.f);

			UpdateConfigData();
			UI->Draw();
			UpdateCamera();
			draw->End();
		}
	}
};

